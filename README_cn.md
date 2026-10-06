![CI](https://github.com/ziyaolu123/latex-calculator/actions/workflows/ci.yml/badge.svg)
![License](https://img.shields.io/github/license/ziyaolu123/latex-calculator)
![C++](https://img.shields.io/badge/C%2B%2B-17-blue)

# LaTeX 计算器

[English](README.md) | **简体中文**

用 C++17 写的 LaTeX 数学表达式计算器，零第三方依赖。

### 特性

- 解析核心 LaTeX 数学语法：`\frac{}{}`、`\sqrt{}`、`\sqrt[n]{}`、`^`、括号分组
- 支持三角 / 反三角 / 双曲函数、对数、指数、绝对值
- 内置常量：`\pi` `\tau` `\e` `\phi`
- 隐式乘法：`2\pi`、`2(3+4)`、`\frac{1}{2}x`
- 自动忽略排版命令：`\left` `\right` `\displaystyle` `\quad` 等
- 交互式 REPL，支持**历史记录（↑/↓）**和**光标移动（←/→、Home/End、Delete）**
- 清晰的错误提示（括号不匹配、除以零、定义域越界等）

### 构建

需要 CMake 3.16+ 和支持 C++17 的编译器。

    git clone https://github.com/ziyaolu123/latex-calculator.git
    cd latex-calculator
    cmake -B build -DCMAKE_BUILD_TYPE=Release
    cmake --build build -j

产物在 `build/latexcalc`（Windows 下是 `build/Release/latexcalc.exe`）。

### 用法

#### 单次计算

    ./build/latexcalc "\frac{1}{2} + \sqrt{3}"
    1.36602540378444

    ./build/latexcalc "\sqrt[3]{27} + 2^{10}"
    1027

#### 交互模式

    ./build/latexcalc
    > \frac{1}{2} + \frac{1}{3}
    = 0.833333333333333
    > :help
    > :quit

REPL 快捷键：

| 按键 | 功能 |
| --- | --- |
| `↑` / `↓` | 浏览历史记录 |
| `←` / `→` | 移动光标 |
| `Home` / `End` | 跳到行首 / 行尾 |
| `Backspace` / `Delete` | 删除光标前 / 处字符 |
| `Enter` | 求值 |
| `Ctrl+C` | 清空当前行 |
| `Ctrl+D` | 退出（在空行时） |

### 支持的语法

| 类别 | 语法 |
| --- | --- |
| 运算符 | `+` `-` `*` `/` `^` |
| 分数 | `\frac{1}{2}` |
| 平方根 | `\sqrt{2}` |
| n 次根 | `\sqrt[3]{8}` |
| 三角函数 | `\sin \cos \tan \cot \sec \csc` |
| 反三角 | `\arcsin \arccos \arctan` |
| 双曲函数 | `\sinh \cosh \tanh` |
| 对数/指数 | `\ln \log \lg \exp \abs` |
| 常量 | `\pi \tau \e \phi` |

### 测试

    ctest --test-dir build --output-on-failure

### 许可证

MIT

### 更新日志

- [English](CHANGELOG.md)
- [简体中文](CHANGELOG_cn.md)