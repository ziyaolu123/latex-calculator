#pragma once
#include <string>

namespace latexcalc {

std::string calculate(const std::string& latex);
std::string formatNumber(double v);

} // namespace latexcalc
