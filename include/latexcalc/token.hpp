#pragma once
#include <cstddef>
#include <string>

namespace latexcalc {

enum class TokenType {
    Number,
    Command,
    Plus, Minus, Times, Div, Caret,
    LParen, RParen,
    LBrace, RBrace,
    LBracket, RBracket,
    End
};

struct Token {
    TokenType   type = TokenType::End;
    std::string text;
    double      value = 0.0;
    std::size_t pos   = 0;
};

std::string tokenName(const Token& t);

} // namespace latexcalc
