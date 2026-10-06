#include "latexcalc/calculator.hpp"
#include "latexcalc/error.hpp"
#include "latexcalc/i18n.hpp"
#include "latexcalc/line_editor.hpp"
#include <iostream>
#include <string>

using latexcalc::i18n::Lang;
namespace i18n = latexcalc::i18n;

namespace {

void printHelp() {
    std::cout << i18n::tr(
        "示例：\n"
        "  \\frac{1}{2} + \\sqrt{3}\n"
        "  \\sin{\\pi \\div 2} + \\cos{0}\n"
        "  \\sqrt[3]{27} + 2^{10}\n"
        "\n"
        "运算符：+ - \\times \\div ^\n"
        "结构：\\frac{}{} \\sqrt{} \\sqrt[n]{}  () {} []\n"
        "函数：\\sin \\cos \\tan \\cot \\sec \\csc\n"
        "      \\arcsin \\arccos \\arctan\n"
        "      \\sinh \\cosh \\tanh \\ln \\log \\lg \\exp \\abs\n"
        "常量：\\pi \\tau \\e \\phi\n"
        "\n"
        "快捷键：↑/↓ 历史，←/→ 光标，Home/End 行首尾，Ctrl+C 清行，Ctrl+D 退出",
        "Examples:\n"
        "  \\frac{1}{2} + \\sqrt{3}\n"
        "  \\sin{\\pi \\div 2} + \\cos{0}\n"
        "  \\sqrt[3]{27} + 2^{10}\n"
        "\n"
        "Operators: + - \\times \\div ^\n"
        "Structures: \\frac{}{} \\sqrt{} \\sqrt[n]{}  () {} []\n"
        "Functions: \\sin \\cos \\tan \\cot \\sec \\csc\n"
        "           \\arcsin \\arccos \\arctan\n"
        "           \\sinh \\cosh \\tanh \\ln \\log \\lg \\exp \\abs\n"
        "Constants: \\pi \\tau \\e \\phi\n"
        "\n"
        "Keys: Up/Down history, Left/Right cursor, Home/End, Ctrl+C clear, Ctrl+D exit"
    ) << "\n";
}

int runOne(const std::string& expr) {
    try {
        std::cout << latexcalc::calculate(expr) << std::endl;
        return 0;
    } catch (const latexcalc::LatexError& e) {
        std::cerr << i18n::tr("错误: ", "error: ") << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << i18n::tr("未知错误: ", "unexpected error: ")
                  << e.what() << std::endl;
        return 1;
    }
}

} // namespace

int main(int argc, char** argv) {
    std::string expr;
    bool langSet = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if ((arg == "--lang" || arg == "-l") && i + 1 < argc) {
            Lang l;
            if (i18n::parseLang(argv[++i], l)) { i18n::setLang(l); langSet = true; }
        } else if (arg.rfind("--lang=", 0) == 0) {
            Lang l;
            if (i18n::parseLang(arg.substr(7), l)) { i18n::setLang(l); langSet = true; }
        } else {
            if (!expr.empty()) expr += ' ';
            expr += arg;
        }
    }
    if (!langSet) i18n::setLang(i18n::detectFromEnv());

    if (!expr.empty()) return runOne(expr);

    std::cout << i18n::tr(
        "LaTeX 计算器 v2.1.0\n"
        "输入 LaTeX 数学表达式回车计算，:help 帮助，:quit 退出。",
        "LaTeX Calculator v2.1.0\n"
        "Type a LaTeX expression and press Enter. :help for help, :quit to exit."
    ) << "\n\n";

    latexcalc::LineEditor editor("> ");

    while (true) {
        auto input = editor.readLine();
        if (!input) break;

        std::string line = *input;
        auto start = line.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) continue;
        auto end = line.find_last_not_of(" \t\r\n");
        line = line.substr(start, end - start + 1);
        if (line.empty()) continue;
        if (line == ":quit" || line == ":q" || line == "exit") break;
        if (line == ":help" || line == ":h") { printHelp(); continue; }

        try {
            std::cout << "= " << latexcalc::calculate(line) << '\n';
            editor.addHistory(line);
        } catch (const latexcalc::LatexError& e) {
            std::cout << i18n::tr("错误: ", "error: ") << e.what() << '\n';
        } catch (const std::exception& e) {
            std::cout << i18n::tr("未知错误: ", "unexpected error: ")
                      << e.what() << '\n';
        }
    }

    return 0;
}
