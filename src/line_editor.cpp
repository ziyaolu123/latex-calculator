#include "latexcalc/line_editor.hpp"

#include <cstdio>
#include <iostream>
#include <string>

#if defined(_WIN32)
#  include <conio.h>
#  include <windows.h>
#else
#  include <termios.h>
#  include <unistd.h>
#endif

namespace latexcalc {

namespace {

// ---------- 终端原始模式 ----------

#if !defined(_WIN32)
termios g_origTermios;
bool    g_rawEnabled = false;

bool stdinIsTty() { return isatty(STDIN_FILENO) != 0; }

void enableRaw() {
    if (g_rawEnabled || !stdinIsTty()) return;
    tcgetattr(STDIN_FILENO, &g_origTermios);
    termios raw = g_origTermios;
    raw.c_lflag &= ~(ICANON | ECHO | ISIG | IEXTEN);
    raw.c_iflag &= ~(IXON | ICRNL);
    raw.c_cc[VMIN]  = 1;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    g_rawEnabled = true;
}

void disableRaw() {
    if (!g_rawEnabled) return;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &g_origTermios);
    g_rawEnabled = false;
}
#else
DWORD g_origInMode  = 0;
DWORD g_origOutMode = 0;
bool  g_rawEnabled  = false;

bool stdinIsTty() {
    HANDLE h = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode = 0;
    return h != INVALID_HANDLE_VALUE && GetConsoleMode(h, &mode);
}

void enableRaw() {
    if (g_rawEnabled || !stdinIsTty()) return;
    HANDLE hIn  = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleMode(hIn,  &g_origInMode);
    GetConsoleMode(hOut, &g_origOutMode);

    DWORD inMode = g_origInMode;
    inMode &= ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_PROCESSED_INPUT);
    inMode |= ENABLE_VIRTUAL_TERMINAL_INPUT;
    SetConsoleMode(hIn, inMode);

    DWORD outMode = g_origOutMode;
    outMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, outMode);

    g_rawEnabled = true;
}

void disableRaw() {
    if (!g_rawEnabled) return;
    HANDLE hIn  = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleMode(hIn,  g_origInMode);
    SetConsoleMode(hOut, g_origOutMode);
    g_rawEnabled = false;
}
#endif

// ---------- 按键解析 ----------

enum class Key {
    Char, Enter, Backspace, Delete,
    Left, Right, Up, Down, Home, End,
    CtrlC, CtrlD, Unknown
};

struct KeyEvent {
    Key  key;
    char ch;
};

#if defined(_WIN32)
KeyEvent readKey() {
    int c = _getch();
    if (c == 0 || c == 0xE0) {
        int c2 = _getch();
        switch (c2) {
            case 72: return {Key::Up,      0};
            case 80: return {Key::Down,    0};
            case 75: return {Key::Left,    0};
            case 77: return {Key::Right,   0};
            case 71: return {Key::Home,    0};
            case 79: return {Key::End,     0};
            case 83: return {Key::Delete,  0};
            default: return {Key::Unknown, 0};
        }
    }
    if (c == '\r' || c == '\n') return {Key::Enter,     0};
    if (c == '\b' || c == 0x7f) return {Key::Backspace, 0};
    if (c == 0x03)              return {Key::CtrlC,     0};
    if (c == 0x04)              return {Key::CtrlD,     0};
    if (c < 0 || c > 255)       return {Key::Unknown,   0};
    return {Key::Char, static_cast<char>(c)};
}
#else
KeyEvent readKey() {
    char c;
    if (::read(STDIN_FILENO, &c, 1) != 1) return {Key::CtrlD, 0};

    if (c == '\x1b') {
        char seq[2];
        if (::read(STDIN_FILENO, &seq[0], 1) != 1) return {Key::Unknown, 0};
        if (seq[0] != '[' && seq[0] != 'O')       return {Key::Unknown, 0};
        if (::read(STDIN_FILENO, &seq[1], 1) != 1) return {Key::Unknown, 0};

        if (seq[0] == 'O') {
            switch (seq[1]) {
                case 'H': return {Key::Home, 0};
                case 'F': return {Key::End,  0};
                default:  return {Key::Unknown, 0};
            }
        }
        switch (seq[1]) {
            case 'A': return {Key::Up,    0};
            case 'B': return {Key::Down,  0};
            case 'C': return {Key::Right, 0};
            case 'D': return {Key::Left,  0};
            case 'H': return {Key::Home,  0};
            case 'F': return {Key::End,   0};
            case '1':
            case '4':
            case '3': {
                char tilde;
                if (::read(STDIN_FILENO, &tilde, 1) != 1 || tilde != '~')
                    return {Key::Unknown, 0};
                if (seq[1] == '3') return {Key::Delete, 0};
                return {seq[1] == '1' ? Key::Home : Key::End, 0};
            }
            default: return {Key::Unknown, 0};
        }
    }

    if (c == '\r' || c == '\n') return {Key::Enter,     0};
    if (c == 0x7f || c == '\b') return {Key::Backspace, 0};
    if (c == 0x03)              return {Key::CtrlC,     0};
    if (c == 0x04)              return {Key::CtrlD,     0};
    return {Key::Char, c};
}
#endif

} // namespace

// ---------- LineEditor ----------

LineEditor::LineEditor(std::string prompt) : prompt_(std::move(prompt)) {}

void LineEditor::addHistory(const std::string& line) {
    if (line.empty()) return;
    if (!history_.empty() && history_.back() == line) return;

    // 去重：如果已存在，移到末尾
    for (auto it = history_.begin(); it != history_.end(); ++it) {
        if (*it == line) { history_.erase(it); break; }
    }
    history_.push_back(line);

    constexpr std::size_t kMaxHistory = 500;
    if (history_.size() > kMaxHistory) {
        history_.erase(history_.begin(),
                       history_.begin() + (history_.size() - kMaxHistory));
    }
}

void LineEditor::clearHistory() { history_.clear(); }

void LineEditor::redraw(const std::string& buf, int cursor) const {
    std::string out;
    out.reserve(prompt_.size() + buf.size() + 16);
    out += '\r';
    out += prompt_;
    out += buf;
    out += "\x1b[K";                    // 清除行尾残留
    int back = static_cast<int>(buf.size()) - cursor;
    if (back > 0) {
        out += "\x1b[";
        out += std::to_string(back);
        out += 'D';                     // 光标左移
    }
    std::cout << out << std::flush;
}

std::optional<std::string> LineEditor::readLine() {
    // 非 TTY 时退化为普通 getline
    if (!stdinIsTty()) {
        std::string line;
        if (!std::getline(std::cin, line)) return std::nullopt;
        return line;
    }

    enableRaw();

    std::string buf;
    int         cursor    = 0;
    int         histIndex = -1;
    std::string savedCurrent;

    redraw(buf, cursor);

    while (true) {
        KeyEvent ev = readKey();
        bool needRedraw = true;

        switch (ev.key) {
            case Key::Char:
                buf.insert(buf.begin() + cursor, ev.ch);
                ++cursor;
                break;

            case Key::Backspace:
                if (cursor > 0) { buf.erase(cursor - 1, 1); --cursor; }
                break;

            case Key::Delete:
                if (cursor < static_cast<int>(buf.size())) buf.erase(cursor, 1);
                break;

            case Key::Left:
                if (cursor > 0) --cursor;
                break;

            case Key::Right:
                if (cursor < static_cast<int>(buf.size())) ++cursor;
                break;

            case Key::Home: cursor = 0; break;
            case Key::End:  cursor = static_cast<int>(buf.size()); break;

            case Key::Up: {
                if (history_.empty()) break;
                if (histIndex == -1) {
                    savedCurrent = buf;
                    histIndex    = static_cast<int>(history_.size()) - 1;
                } else if (histIndex > 0) {
                    --histIndex;
                }
                buf    = history_[histIndex];
                cursor = static_cast<int>(buf.size());
                break;
            }

            case Key::Down: {
                if (histIndex == -1) break;
                if (histIndex < static_cast<int>(history_.size()) - 1) {
                    ++histIndex;
                    buf = history_[histIndex];
                } else {
                    histIndex = -1;
                    buf       = savedCurrent;
                }
                cursor = static_cast<int>(buf.size());
                break;
            }

            case Key::Enter:
                disableRaw();
                std::cout << '\n' << std::flush;
                return buf;

            case Key::CtrlC:
                disableRaw();
                std::cout << "^C\n" << std::flush;
                return std::string{};       // 返回空串，调用方跳过

            case Key::CtrlD:
                if (buf.empty()) {
                    disableRaw();
                    std::cout << '\n' << std::flush;
                    return std::nullopt;    // EOF
                }
                if (cursor < static_cast<int>(buf.size())) buf.erase(cursor, 1);
                break;

            case Key::Unknown:
                needRedraw = false;
                break;
        }

        if (needRedraw) redraw(buf, cursor);
    }
}

} // namespace latexcalc