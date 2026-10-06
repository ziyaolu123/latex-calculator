#include "latexcalc/lexer.hpp"
#include "latexcalc/error.hpp"
#include "latexcalc/i18n.hpp"
#include <cctype>
#include <cstdlib>
#include <unordered_set>

namespace latexcalc {

namespace {
const std::unordered_set<std::string> kIgnoredCommands = {
    "left","right","displaystyle","textstyle",
    "big","Big","bigg","Bigg","quad","qquad"
};
}

Lexer::Lexer(std::string src) : src_(std::move(src)) {}

char Lexer::peekChar(std::size_t off) const {
    std::size_t i = pos_ + off;
    return i < src_.size() ? src_[i] : '\0';
}

void Lexer::skipWhitespace() {
    while (pos_ < src_.size()) {
        char c = src_[pos_];
        if (c==' '||c=='\t'||c=='\n'||c=='\r'||c=='\f'||c=='\v') ++pos_;
        else break;
    }
}

Token Lexer::readNumber() {
    std::size_t start = pos_;
    bool hasDot = false;
    while (pos_ < src_.size()) {
        char c = src_[pos_];
        if (std::isdigit(static_cast<unsigned char>(c))) ++pos_;
        else if (c == '.' && !hasDot) { hasDot = true; ++pos_; }
        else break;
    }
    Token t;
    t.type  = TokenType::Number;
    t.text  = src_.substr(start, pos_ - start);
    t.value = std::strtod(t.text.c_str(), nullptr);
    t.pos   = start;
    return t;
}

Token Lexer::readCommand() {
    std::size_t start = pos_;
    ++pos_;
    std::string name;
    while (pos_ < src_.size() &&
           std::isalpha(static_cast<unsigned char>(src_[pos_]))) {
        name += src_[pos_++];
    }
    Token t;
    t.type = TokenType::Command;
    t.text = name;
    t.pos  = start;
    return t;
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    while (true) {
        skipWhitespace();
        if (pos_ >= src_.size()) {
            Token end; end.type = TokenType::End; end.pos = pos_;
            tokens.push_back(end);
            break;
        }
        char c = src_[pos_];
        if (std::isdigit(static_cast<unsigned char>(c)) ||
            (c=='.' && std::isdigit(static_cast<unsigned char>(peekChar(1))))) {
            tokens.push_back(readNumber());
            continue;
        }
        if (c == '\\') {
            Token t = readCommand();
            if (t.text.empty()) {
                if (pos_ < src_.size()) ++pos_;
                continue;
            }
            if (kIgnoredCommands.count(t.text)) continue;

            // \times → Times，\div → Div
            if (t.text == "times") {
                t.type = TokenType::Times;
                tokens.push_back(t);
                continue;
            }
            if (t.text == "div") {
                t.type = TokenType::Div;
                tokens.push_back(t);
                continue;
            }

            tokens.push_back(t);
            continue;
        }
        Token t; t.pos = pos_;
        switch (c) {
            case '+': t.type=TokenType::Plus;     t.text="+"; ++pos_; break;
            case '-': t.type=TokenType::Minus;    t.text="-"; ++pos_; break;
            case '^': t.type=TokenType::Caret;    t.text="^"; ++pos_; break;
            case '(': t.type=TokenType::LParen;   t.text="("; ++pos_; break;
            case ')': t.type=TokenType::RParen;   t.text=")"; ++pos_; break;
            case '{': t.type=TokenType::LBrace;   t.text="{"; ++pos_; break;
            case '}': t.type=TokenType::RBrace;   t.text="}"; ++pos_; break;
            case '[': t.type=TokenType::LBracket; t.text="["; ++pos_; break;
            case ']': t.type=TokenType::RBracket; t.text="]"; ++pos_; break;
            default:
              throw LexError(i18n::tr(
                  "无法识别的字符 '{}'（位置 {}）。乘除法请使用 \\times 和 \\div",
                  "unrecognized character '{}' at position {}. Use \\times and \\div for multiplication and division.",
                  { std::string(1, c), std::to_string(pos_) }));
        }
        tokens.push_back(t);
    }
    return tokens;
}

std::string tokenName(const Token& t) {
    switch (t.type) {
        case TokenType::Number:   return "数字 '" + t.text + "'";
        case TokenType::Command:  return "命令 '\\" + t.text + "'";
        case TokenType::Plus:     return "'+'";
        case TokenType::Minus:    return "'-'";
        case TokenType::Times:    return "'\\times'";
        case TokenType::Div:      return "'\\div'";
        case TokenType::Caret:    return "'^'";
        case TokenType::LParen:   return "'('";
        case TokenType::RParen:   return "')'";
        case TokenType::LBrace:   return "'{'";
        case TokenType::RBrace:   return "'}'";
        case TokenType::LBracket: return "'['";
        case TokenType::RBracket: return "']'";
        case TokenType::End:      return "表达式末尾";
    }
    return "未知符号";
}

} // namespace latexcalc
