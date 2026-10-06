#pragma once
#include <optional>
#include <string>
#include <vector>

namespace latexcalc {

// 支持历史记录（↑/↓）与光标移动（←/→/Home/End/Delete）的行编辑器
class LineEditor {
public:
    explicit LineEditor(std::string prompt);

    // 返回 std::nullopt 表示 EOF（Ctrl+D）
    // 返回空字符串表示用户按了 Ctrl+C 取消，调用方可跳过
    std::optional<std::string> readLine();

    void addHistory(const std::string& line);
    void clearHistory();

private:
    void redraw(const std::string& buf, int cursor) const;

    std::string              prompt_;
    std::vector<std::string> history_;
};

} // namespace latexcalc