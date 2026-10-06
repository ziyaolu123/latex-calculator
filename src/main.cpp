#include "latexcalc/calculator.hpp"
#include "latexcalc/error.hpp"
#include "latexcalc/line_editor.hpp"
#include <iostream>
#include <string>

namespace {

const char* kBanner =
    "LaTeX Calculator v2.0.0\n"
    "输入 LaTeX 数学表达式回车计算，:help 帮助，:quit 退出。\n";

void printHelp() {
    std::cout <<
        "示例：\n"
        "  \\frac{1}{2} + \\sqrt{3}\n"
        "  \\sin{\\pi/6} + \\cos{0}\n"
        "  \\sqrt[3]{27} + 2^{10}\n"
        "  \\ln{\\e} + \\log{1000}\n"
        "\n"
        "运算符：+ - \\times \\div ^   （支持隐式乘法：2\\pi、2(3+4)）\n"
        "结构：\\frac{}{} \\sqrt{} \\sqrt[n]{}  () {} []\n"
        "函数：\\sin \\cos \\tan \\cot \\sec \\csc\n"
        "      \\arcsin \\arccos \\arctan\n"
        "      \\sinh \\cosh \\tanh \\ln \\log \\lg \\exp \\abs\n"
        "常量：\\pi \\tau \\e \\phi\n"
        "\n"
        "快捷键：↑/↓ 历史记录，←/→ 移动光标，Home/End 行首行尾\n"
        "        Ctrl+C 清空当前行，Ctrl+D 退出\n";
}

int runOne(const std::string& expr) {
    try {
        std::cout << latexcalc::calculate(expr) << std::endl;
        return 0;
    } catch (const latexcalc::LatexError& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "未知错误: " << e.what() << std::endl;
        return 1;
    }
}

} // namespace

int main(int argc, char** argv) {
    if (argc > 1) {
        std::string expr;
        for (int i = 1; i < argc; ++i) {
            if (i > 1) expr += ' ';
            expr += argv[i];
        }
        return runOne(expr);
    }

    std::cout << kBanner << std::endl;

    latexcalc::LineEditor editor("> ");
    std::string line;

    while (true) {
        auto input = editor.readLine();
        if (!input) break;              // Ctrl+D

        line = *input;
        auto start = line.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) continue;
        auto end = line.find_last_not_of(" \t\r\n");
        line = line.substr(start, end - start + 1);

        if (line.empty()) continue;

        if (line == ":quit" || line == ":q" || line == "exit") break;
        if (line == ":help" || line == ":h") { printHelp(); continue; }

        try {
            std::cout << "= " << latexcalc::calculate(line) << '\n';
            editor.addHistory(line);    // 只有成功执行才记入历史
        } catch (const latexcalc::LatexError& e) {
            std::cout << "错误: " << e.what() << '\n';
        } catch (const std::exception& e) {
            std::cout << "未知错误: " << e.what() << '\n';
        }
    }

    return 0;
}