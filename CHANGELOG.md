# Changelog

**English** | [简体中文](CHANGELOG_cn.md)

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [1.0.1] - 2026-10-06

### Added
- Interactive REPL with line editing:
  - History navigation via `↑` / `↓`
  - Cursor movement via `←` / `→`, `Home` / `End`
  - `Delete` and `Backspace` at cursor position
  - `Ctrl+C` clears the current line
  - `Ctrl+D` exits on an empty line
- Bilingual documentation: `README.md` (English) and `README_cn.md` (简体中文)
- CI, license, and C++17 badges in both READMEs

## [1.0.0] - 2026-10-06

### Added
- Initial release.
- LaTeX parser and evaluator supporting:
  - Operators: `+` `-` `*` `/` `^`
  - Structures: `\frac{}{}`, `\sqrt{}`, `\sqrt[n]{}`, grouping with `()` `{}`
  - Functions: `\sin` `\cos` `\tan` `\cot` `\sec` `\csc`
  - Inverse trig: `\arcsin` `\arccos` `\arctan`
  - Hyperbolic: `\sinh` `\cosh` `\tanh`
  - Logs / exp: `\ln` `\log` `\lg` `\exp` `\abs`
  - Constants: `\pi` `\tau` `\e` `\phi`
- Implicit multiplication: `2\pi`, `2(3+4)`, `\frac{1}{2}x`
- Silent ignore of typography commands: `\left` `\right` `\displaystyle` `\quad`
- Clear error messages for unmatched brackets, division by zero, and domain violations
- CMake build with CTest-based unit tests
- GitHub Actions CI matrix: Ubuntu, macOS, Windows
- MIT License

[Unreleased]: https://github.com/ziyaolu123/latex-calculator/compare/v1.0.1...HEAD
[1.0.1]: https://github.com/ziyaolu123/latex-calculator/compare/v1.0.0...v1.0.1
[1.0.0]: https://github.com/ziyaolu123/latex-calculator/releases/tag/v1.0.0
