# 更新日志

本项目所有值得记录的变更都会写在此文件中。

格式参考 [Keep a Changelog](https://keepachangelog.com/zh-CN/1.1.0/)，
版本号遵循 [语义化版本](https://semver.org/lang/zh-CN/)。

[English](CHANGELOG.md) | **简体中文**

## [未发布]

## [1.0.1] - 2026-10-06

### 新增
- 交互式 REPL 行编辑功能：
  - 通过 `↑` / `↓` 浏览历史记录
  - 通过 `←` / `→`、`Home` / `End` 移动光标
  - 支持在光标位置 `Delete` 和 `Backspace`
  - `Ctrl+C` 清空当前行
  - `Ctrl+D` 在空行退出
- 双语文档：`README.md`（English）和 `README_cn.md`（简体中文）
- 两个 README 都加入 CI、License、C++17 徽章

## [1.0.0] - 2026-10-06

### 新增
- 首个正式版本。
- LaTeX 解析与求值，支持：
  - 运算符：`+` `-` `*` `/` `^`
  - 结构：`\frac{}{}`、`\sqrt{}`、`\sqrt[n]{}`、用 `()` `{}` 分组
  - 三角函数：`\sin` `\cos` `\tan` `\cot` `\sec` `\csc`
  - 反三角：`\arcsin` `\arccos` `\arctan`
  - 双曲函数：`\sinh` `\cosh` `\tanh`
  - 对数与指数：`\ln` `\log` `\lg` `\exp` `\abs`
  - 常量：`\pi` `\tau` `\e` `\phi`
- 隐式乘法：`2\pi`、`2(3+4)`、`\frac{1}{2}x`
- 自动忽略排版命令：`\left` `\right` `\displaystyle` `\quad`
- 清晰的错误提示：括号不匹配、除以零、定义域越界
- 基于 CMake 构建，CTest 单元测试
- GitHub Actions 三平台 CI：Ubuntu、macOS、Windows
- MIT 许可证

[未发布]: https://github.com/ziyaolu123/latex-calculator/compare/v1.0.1...HEAD
[1.0.1]: https://github.com/ziyaolu123/latex-calculator/compare/v1.0.0...v1.0.1
[1.0.0]: https://github.com/ziyaolu123/latex-calculator/releases/tag/v1.0.0
