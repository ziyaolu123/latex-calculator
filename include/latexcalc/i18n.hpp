#pragma once
#include <cctype>
#include <cstdlib>
#include <initializer_list>
#include <string>
#include <string_view>

namespace latexcalc::i18n {

enum class Lang { En, Zh };

inline Lang g_lang = Lang::En;
inline void setLang(Lang l) { g_lang = l; }
inline Lang current() { return g_lang; }

inline bool parseLang(std::string_view s, Lang& out) {
    if (s.size() < 2) return false;
    auto low = [](char c) {
        return static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    };
    char a = low(s[0]), b = low(s[1]);
    if (a == 'e' && b == 'n') { out = Lang::En; return true; }
    if (a == 'z' && b == 'h') { out = Lang::Zh; return true; }
    return false;
}

inline Lang detectFromEnv() {
    for (const char* name : {"LATEXCALC_LANG", "LC_ALL", "LANG"}) {
        if (const char* v = std::getenv(name)) {
            Lang l;
            if (parseLang(v, l)) return l;
        }
    }
    return Lang::En;
}

inline std::string substitute(std::string tpl,
                              std::initializer_list<std::string> args) {
    auto it = args.begin();
    std::size_t pos = 0;
    while (it != args.end()) {
        std::size_t open = tpl.find('{', pos);
        if (open == std::string::npos) break;
        std::size_t close = tpl.find('}', open);
        if (close == std::string::npos) break;
        tpl.replace(open, close - open + 1, *it);
        pos = open + it->size();
        ++it;
    }
    return tpl;
}

inline std::string tr(std::string_view zh, std::string_view en) {
    return std::string(current() == Lang::Zh ? zh : en);
}

inline std::string tr(std::string_view zh, std::string_view en,
                      std::initializer_list<std::string> args) {
    return substitute(
        current() == Lang::Zh ? std::string(zh) : std::string(en), args);
}

} // namespace latexcalc::i18n
