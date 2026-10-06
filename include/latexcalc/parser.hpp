#pragma once
#include "latexcalc/ast.hpp"
#include "latexcalc/token.hpp"
#include <vector>

namespace latexcalc {

class Parser {
public:
    explicit Parser(std::vector<Token> tokens);
    ExprPtr parse();

private:
    const Token& peek() const;
    bool  check(TokenType t) const;
    bool  match(TokenType t);
    Token advance();
    const Token& expect(TokenType t, const std::string& what);

    bool    startsPrimary() const;
    ExprPtr parseExpression();
    ExprPtr parseTerm();
    ExprPtr parseUnary();
    ExprPtr parsePower();
    ExprPtr parsePrimary();
    ExprPtr parseCommand();
    ExprPtr parseArgument();

    std::vector<Token> tokens_;
    std::size_t pos_ = 0;
};

} // namespace latexcalc
