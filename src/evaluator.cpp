#include "latexcalc/evaluator.hpp"
#include "latexcalc/error.hpp"
#include <cmath>
#include <string>

namespace latexcalc {

namespace {

double evalNode(const Expr* e);

double evalFunction(const std::string& name, double x) {
    if (name == "sin")    return std::sin(x);
    if (name == "cos")    return std::cos(x);
    if (name == "tan")    return std::tan(x);
    if (name == "cot") {
        double t = std::tan(x);
        if (t == 0.0) throw EvalError("cot 未定义（tan = 0）");
        return 1.0 / t;
    }
    if (name == "sec") {
        double c = std::cos(x);
        if (c == 0.0) throw EvalError("sec 未定义（cos = 0）");
        return 1.0 / c;
    }
    if (name == "csc") {
        double s = std::sin(x);
        if (s == 0.0) throw EvalError("csc 未定义（sin = 0）");
        return 1.0 / s;
    }
    if (name == "arcsin") {
        if (x < -1.0 || x > 1.0) throw EvalError("arcsin 的定义域是 [-1, 1]");
        return std::asin(x);
    }
    if (name == "arccos") {
        if (x < -1.0 || x > 1.0) throw EvalError("arccos 的定义域是 [-1, 1]");
        return std::acos(x);
    }
    if (name == "arctan") return std::atan(x);
    if (name == "sinh")   return std::sinh(x);
    if (name == "cosh")   return std::cosh(x);
    if (name == "tanh")   return std::tanh(x);
    if (name == "ln") {
        if (x <= 0.0) throw EvalError("ln 的定义域是正实数");
        return std::log(x);
    }
    if (name == "log" || name == "lg") {
        if (x <= 0.0) throw EvalError("log 的定义域是正实数");
        return std::log10(x);
    }
    if (name == "exp") return std::exp(x);
    if (name == "abs") return std::fabs(x);
    throw EvalError("未知函数 \\" + name);
}

double evalBinary(const BinaryExpr* b) {
    double l = evalNode(b->lhs.get());
    double r = evalNode(b->rhs.get());
    switch (b->op) {
        case BinOp::Add: return l + r;
        case BinOp::Sub: return l - r;
        case BinOp::Mul: return l * r;
        case BinOp::Div:
            if (r == 0.0) throw EvalError("除以零");
            return l / r;
        case BinOp::Pow: {
            double res = std::pow(l, r);
            if (std::isnan(res)) throw EvalError("幂运算结果未定义");
            return res;
        }
    }
    throw EvalError("内部错误：未知二元运算符");
}

double evalNode(const Expr* e) {
    if (auto n = dynamic_cast<const NumberExpr*>(e)) return n->value;
    if (auto b = dynamic_cast<const BinaryExpr*>(e)) return evalBinary(b);
    if (auto u = dynamic_cast<const UnaryExpr*>(e)) {
        double v = evalNode(u->operand.get());
        return u->op == UnOp::Minus ? -v : v;
    }
    if (auto f = dynamic_cast<const FractionExpr*>(e)) {
        double n = evalNode(f->numerator.get());
        double d = evalNode(f->denominator.get());
        if (d == 0.0) throw EvalError("分母为零");
        return n / d;
    }
    if (auto s = dynamic_cast<const SqrtExpr*>(e)) {
        double r = evalNode(s->radicand.get());
        if (!s->degree) {
            if (r < 0.0) throw EvalError("负数不能开平方（实数范围内）");
            return std::sqrt(r);
        }
        double n = evalNode(s->degree.get());
        if (n == 0.0) throw EvalError("根指数不能为零");
        if (r < 0.0) {
            long long ni = std::llround(n);
            bool odd = (std::fabs(n - static_cast<double>(ni)) < 1e-12) && (ni % 2 != 0);
            if (odd) return -std::pow(-r, 1.0 / n);
            throw EvalError("负数不能开偶次方根（实数范围内）");
        }
        return std::pow(r, 1.0 / n);
    }
    if (auto fn = dynamic_cast<const FunctionExpr*>(e)) {
        double arg = evalNode(fn->arg.get());
        return evalFunction(fn->name, arg);
    }
    throw EvalError("内部错误：未知表达式节点");
}

}

double evaluate(const Expr* expr) {
    if (!expr) throw EvalError("表达式为空");
    return evalNode(expr);
}

} // namespace latexcalc
