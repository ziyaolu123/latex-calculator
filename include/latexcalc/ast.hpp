#pragma once
#include <memory>
#include <string>

namespace latexcalc {

enum class BinOp { Add, Sub, Mul, Div, Pow };
enum class UnOp  { Plus, Minus };

struct Expr { virtual ~Expr() = default; };
using ExprPtr = std::unique_ptr<Expr>;

struct NumberExpr : Expr {
    double value;
    explicit NumberExpr(double v) : value(v) {}
};

struct BinaryExpr : Expr {
    BinOp   op;
    ExprPtr lhs, rhs;
    BinaryExpr(BinOp o, ExprPtr l, ExprPtr r)
        : op(o), lhs(std::move(l)), rhs(std::move(r)) {}
};

struct UnaryExpr : Expr {
    UnOp    op;
    ExprPtr operand;
    UnaryExpr(UnOp o, ExprPtr e) : op(o), operand(std::move(e)) {}
};

struct FractionExpr : Expr {
    ExprPtr numerator, denominator;
    FractionExpr(ExprPtr n, ExprPtr d)
        : numerator(std::move(n)), denominator(std::move(d)) {}
};

struct SqrtExpr : Expr {
    ExprPtr radicand;
    ExprPtr degree;
    SqrtExpr(ExprPtr r, ExprPtr d)
        : radicand(std::move(r)), degree(std::move(d)) {}
};

struct FunctionExpr : Expr {
    std::string name;
    ExprPtr     arg;
    FunctionExpr(std::string n, ExprPtr a)
        : name(std::move(n)), arg(std::move(a)) {}
};

} // namespace latexcalc
