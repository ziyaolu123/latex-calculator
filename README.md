![CI](https://github.com/ziyaolu123/latex-calculator/actions/workflows/ci.yml/badge.svg)
![License](https://img.shields.io/github/license/ziyaolu123/latex-calculator)
![C++](https://img.shields.io/badge/C%2B%2B-17-blue)

# LaTeX Calculator (C++17)

零依赖的 LaTeX 数学表达式计算器。

## 构建

    cmake -B build -DCMAKE_BUILD_TYPE=Release
    cmake --build build -j

## 运行

    ./build/latexcalc "\frac{1}{2} + \sqrt{3}"
    ./build/latexcalc          # 进入交互 REPL

## 支持语法

| 类别 | 语法 |
| --- | --- |
| 运算符 | `+` `-` `*` `/` `^` |
| 分数 | `\frac{1}{2}` |
| 平方根 | `\sqrt{2}` |
| n 次根 | `\sqrt[3]{8}` |
| 三角函数 | `\sin \cos \tan \cot \sec \csc` |
| 反三角 | `\arcsin \arccos \arctan` |
| 双曲 | `\sinh \cosh \tanh` |
| 对数/指数 | `\ln \log \lg \exp \abs` |
| 常量 | `\pi \tau \e \phi` |

隐式乘法支持：`2\pi`、`2(3+4)`、`\frac{1}{2}4`。
`\left \right \displaystyle \quad` 等排版命令会被忽略。

## 测试

    ctest --test-dir build --output-on-failure

## License

MIT
