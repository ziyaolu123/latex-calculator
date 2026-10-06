#pragma once
#include "latexcalc/token.hpp"
#include <string>
#include <vector>

namespace latexcalc {

class Lexer {
public:
    explicit Lexer(std::string src);
    std::vector<Token> tokenize();

private:
    void  skipWhitespace();
    Token readNumber();
    Token readCommand();
    char  peekChar(std::size_t off = 0) const;

    std::string src_;
    std::size_t pos_ = 0;
};

} // namespace latexcalc
