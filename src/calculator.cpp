#include "latexcalc/calculator.hpp"
#include "latexcalc/lexer.hpp"
#include "latexcalc/parser.hpp"
#include "latexcalc/evaluator.hpp"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>

namespace latexcalc {

std::string formatNumber(double v) {
    if (std::isnan(v)) return "NaN";
    if (std::isinf(v)) return v > 0 ? "+inf" : "-inf";

    double rounded = std::round(v);
    double tol = 1e-12 * std::max(1.0, std::fabs(v));
    if (std::fabs(v - rounded) < tol && std::fabs(v) < 1e15) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(0) << rounded;
        return oss.str();
    }

    std::ostringstream oss;
    oss << std::setprecision(15) << v;
    return oss.str();
}

std::string calculate(const std::string& latex) {
    Lexer lexer(latex);
    auto tokens = lexer.tokenize();
    Parser parser(std::move(tokens));
    auto ast = parser.parse();
    return formatNumber(evaluate(ast.get()));
}

} // namespace latexcalc
