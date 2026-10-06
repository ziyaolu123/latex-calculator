#include "latexcalc/parser.hpp"
#include "latexcalc/error.hpp"
#include <unordered_map>
#include <unordered_set>

namespace latexcalc {

namespace {

const std::unordered_map<std::string, double>& constants() {
    static const std::unordered_map<std::string, double> k = {
        {"pi",  3.14159265358979323846},
        {"tau", 6.28318530717958647692},
        {"e",   2.71828182845904523536},
        {"phi", 1.61803398874989484820},
    };
    return k;
}

bool isFunctionName(const std::string& n) {
    static const std::unordered_set<std::string> k = {
        "sin","cos","tan","cot","sec","csc",
        "arcsin","arccos","arctan",
        "sinh","cosh","tanh",
        "ln","log","lg","exp","abs"
    };
    return k.count(n) > 0;
}

}

Parser::Parser(std::vector<Token> tokens) : tokens_(std::move(tokens)) {}

const Token& Parser::peek() const { return tokens_[pos_]; }
bool Parser::check(TokenType t) const { return peek().type == t; }
bool Parser::match(TokenType t) { if (check(t)) { ++pos_; return true; } return false; }
Token Parser::advance() { return tokens_[pos_++]; }

const Token& Parser::expect(TokenType t, const std::string& what) {
    if (!check(t)) throw ParseError("期望 " + what + "，实际得到 " + tokenName(peek()));
    return tokens_[pos_++];
}

ExprPtr Parser::parse() {
    if (check(TokenType::End)) throw ParseError("表达式为空");
    auto e = parseExpression();
    if (!check(TokenType::End))
        throw ParseError("表达式尾部出现多余内容：" + tokenName(peek()));
    return e;
}

ExprPtr Parser::parseExpression() {
    auto lhs = parseTerm();
    while (true) {
        if (match(TokenType::Plus))
            lhs = std::make_unique<BinaryExpr>(BinOp::Add, std::move(lhs), parseTerm());
        else if (match(TokenType::Minus))
            lhs = std::make_unique<BinaryExpr>(BinOp::Sub, std::move(lhs), parseTerm());
        else break;
    }
    return lhs;
}

bool Parser::startsPrimary() const {
    switch (peek().type) {
        case TokenType::Number:
        case TokenType::Command:
        case TokenType::LParen:
        case TokenType::LBrace: return true;
        default: return false;
    }
}

ExprPtr Parser::parseTerm() {
    auto lhs = parseUnary();
    while (true) {
        if (match(TokenType::Times))
            lhs = std::make_unique<BinaryExpr>(BinOp::Mul, std::move(lhs), parseUnary());
        else if (match(TokenType::Div))
            lhs = std::make_unique<BinaryExpr>(BinOp::Div, std::move(lhs), parseUnary());
        else if (startsPrimary())
            lhs = std::make_unique<BinaryExpr>(BinOp::Mul, std::move(lhs), parseUnary());
        else break;
    }
    return lhs;
}

ExprPtr Parser::parseUnary() {
    if (match(TokenType::Plus))  return parseUnary();
    if (match(TokenType::Minus)) return std::make_unique<UnaryExpr>(UnOp::Minus, parseUnary());
    return parsePower();
}

ExprPtr Parser::parsePower() {
    auto base = parsePrimary();
    if (match(TokenType::Caret)) {
        auto exp = parseUnary();
        return std::make_unique<BinaryExpr>(BinOp::Pow, std::move(base), std::move(exp));
    }
    return base;
}

ExprPtr Parser::parsePrimary() {
    const Token& t = peek();
    switch (t.type) {
        case TokenType::Number: {
            double v = t.value; ++pos_;
            return std::make_unique<NumberExpr>(v);
        }
        case TokenType::LParen: {
            ++pos_;
            auto e = parseExpression();
            expect(TokenType::RParen, "')'");
            return e;
        }
        case TokenType::LBrace: {
            ++pos_;
            auto e = parseExpression();
            expect(TokenType::RBrace, "'}'");
            return e;
        }
        case TokenType::Command:
            return parseCommand();
        default:
            throw ParseError("意外的符号 " + tokenName(t));
    }
}

ExprPtr Parser::parseCommand() {
    Token cmd = advance();
    const std::string& name = cmd.text;

    if (name == "frac") {
        auto num = parseArgument();
        auto den = parseArgument();
        return std::make_unique<FractionExpr>(std::move(num), std::move(den));
    }
    if (name == "sqrt") {
        ExprPtr degree;
        if (match(TokenType::LBracket)) {
            degree = parseExpression();
            expect(TokenType::RBracket, "']'");
        }
        auto radicand = parseArgument();
        return std::make_unique<SqrtExpr>(std::move(radicand), std::move(degree));
    }
    auto cit = constants().find(name);
    if (cit != constants().end())
        return std::make_unique<NumberExpr>(cit->second);

    if (name == "abs" || isFunctionName(name)) {
        auto arg = parseUnary();
        return std::make_unique<FunctionExpr>(name, std::move(arg));
    }
    throw ParseError("未知命令 \\" + name);
}

ExprPtr Parser::parseArgument() {
    if (check(TokenType::LBrace)) {
        ++pos_;
        auto e = parseExpression();
        expect(TokenType::RBrace, "'}'");
        return e;
    }
    if (check(TokenType::LParen)) {
        ++pos_;
        auto e = parseExpression();
        expect(TokenType::RParen, "')'");
        return e;
    }
    return parseUnary();
}

} // namespace latexcalc
