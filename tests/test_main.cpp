#include "latexcalc/calculator.hpp"
#include "latexcalc/error.hpp"
#include <cmath>
#include <iostream>
#include <string>

namespace {

int g_passed = 0;
int g_failed = 0;

void checkNear(const std::string& expr, double expected, double eps = 1e-9) {
    try {
        double got = std::stod(latexcalc::calculate(expr));
        if (std::fabs(got - expected) <= eps) {
            ++g_passed;
        } else {
            ++g_failed;
            std::cerr << "FAIL " << expr << "\n  期望 " << expected
                      << "，实际 " << got << "\n";
        }
    } catch (const std::exception& e) {
        ++g_failed;
        std::cerr << "FAIL " << expr << "\n  抛出异常: " << e.what() << "\n";
    }
}

void checkThrows(const std::string& expr) {
    try {
        std::string s = latexcalc::calculate(expr);
        ++g_failed;
        std::cerr << "FAIL " << expr << " 应当报错，却返回 " << s << "\n";
    } catch (const latexcalc::LatexError&) {
        ++g_passed;
    } catch (...) {
        ++g_failed;
        std::cerr << "FAIL " << expr << " 抛出了非 LatexError 异常\n";
    }
}

}

int main() {
    checkNear("1 + 2", 3);
    checkNear("2 * 3 + 4", 10);
    checkNear("2 + 3 * 4", 14);
    checkNear("(1 + 2) * 3", 9);
    checkNear("10 / 4", 2.5);
    checkNear("2^10", 1024);
    checkNear("2^3^2", 512);
    checkNear("-2^2", -4);
    checkNear("(-2)^2", 4);

    checkNear("\\frac{1}{2}", 0.5);
    checkNear("\\frac{1}{2} + \\frac{1}{3}", 5.0 / 6.0);
    checkNear("\\frac{2}{3} * \\frac{3}{2}", 1.0);
    checkNear("\\frac{\\frac{1}{2}}{2}", 0.25);

    checkNear("\\sqrt{16}", 4);
    checkNear("\\sqrt[3]{27}", 3);
    checkNear("\\sqrt{2}^2", 2);
    checkNear("\\sqrt[3]{-8}", -2);

    checkNear("\\pi", 3.14159265358979323846);
    checkNear("2\\pi", 6.283185307179586);
    checkNear("\\e", 2.718281828459045);
    checkNear("\\tau", 6.283185307179586);

    checkNear("\\sin{0}", 0);
    checkNear("\\cos{0}", 1);
    checkNear("\\sin{\\pi/2}", 1);
    checkNear("\\ln{\\e}", 1);
    checkNear("\\log{1000}", 3);
    checkNear("\\abs{-5}", 5);
    checkNear("\\arctan{1}", 3.14159265358979323846 / 4);

    checkNear("2(3 + 4)", 14);
    checkNear("\\frac{1}{2}4", 2);
    checkNear("\\sqrt{2}\\sqrt{2}", 2);

    checkNear("\\left( 1 + 2 \\right)", 3);

    checkThrows("1 / 0");
    checkThrows("\\frac{1}{0}");
    checkThrows("\\sqrt{-8}");
    checkThrows("\\ln{-1}");
    checkThrows("\\unknowncmd");
    checkThrows("1 +");
    checkThrows("(1 + 2");
    checkThrows("");

    std::cout << "\n通过 " << g_passed << " 项，失败 " << g_failed << " 项\n";
    return g_failed == 0 ? 0 : 1;
}
