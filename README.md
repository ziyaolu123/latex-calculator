![CI](https://github.com/ziyaolu123/latex-calculator/actions/workflows/ci.yml/badge.svg)
![License](https://img.shields.io/github/license/ziyaolu123/latex-calculator)
![C++](https://img.shields.io/badge/C%2B%2B-17-blue)

# LaTeX Calculator

**English** | [简体中文](README_cn.md)

A zero-dependency LaTeX math expression calculator written in C++17.

## Features

- Parses core LaTeX math syntax: `\frac{}{}`, `\sqrt{}`, `\sqrt[n]{}`, `^`, grouping
- Trig / inverse trig / hyperbolic / log / exp / abs functions
- Built-in constants: `\pi` `\tau` `\e` `\phi`
- Implicit multiplication: `2\pi`, `2(3+4)`, `\frac{1}{2}x`
- Silently ignores typography commands: `\left` `\right` `\displaystyle` `\quad`
- Interactive REPL with **history (↑/↓)** and **cursor movement (←/→, Home/End, Delete)**
- Clear error messages for unmatched parens, division by zero, domain violations

## Build

Requires CMake 3.16+ and a C++17 compiler.

    git clone https://github.com/ziyaolu123/latex-calculator.git
    cd latex-calculator
    cmake -B build -DCMAKE_BUILD_TYPE=Release
    cmake --build build -j

Output: `build/latexcalc` (`build/Release/latexcalc.exe` on Windows).

## Run

### One-shot

    ./build/latexcalc "\frac{1}{2} + \sqrt{3}"
    1.36602540378444

    ./build/latexcalc "\sqrt[3]{27} + 2^{10}"
    1027

### Interactive REPL

    ./build/latexcalc
    > \frac{1}{2} + \frac{1}{3}
    = 0.833333333333333
    > :help
    > :quit

REPL key bindings:

| Key | Action |
| --- | --- |
| `↑` / `↓` | Browse history |
| `←` / `→` | Move cursor |
| `Home` / `End` | Jump to line start / end |
| `Backspace` / `Delete` | Delete char before / at cursor |
| `Enter` | Evaluate |
| `Ctrl+C` | Clear current line |
| `Ctrl+D` | Exit (on empty line) |

## Language

The REPL and error messages support English and Chinese.

Priority (highest first):

1. `--lang=en` / `--lang=zh` (also `-l en` / `-l zh`)
2. `LATEXCALC_LANG` environment variable
3. System `LANG` / `LC_ALL`
4. Default: English

```
./build/latexcalc --lang=zh
LATEXCALC_LANG=en ./build/latexcalc
```

## Supported syntax

| Category | Syntax |
| --- | --- |
| Operators | `+` `-` `\times` `\div` `^` |
| Fraction | `\frac{1}{2}` |
| Square root | `\sqrt{2}` |
| n-th root | `\sqrt[3]{8}` |
| Trig | `\sin \cos \tan \cot \sec \csc` |
| Inverse trig | `\arcsin \arccos \arctan` |
| Hyperbolic | `\sinh \cosh \tanh` |
| Log / exp | `\ln \log \lg \exp \abs` |
| Constants | `\pi \tau \e \phi` |

## Test

    ctest --test-dir build --output-on-failure

## License

MIT

## Changelog

- [English](CHANGELOG.md)
- [简体中文](CHANGELOG_cn.md)