#pragma once
#include <stdexcept>
#include <string>

namespace latexcalc {

class LatexError : public std::runtime_error {
public:
    explicit LatexError(const std::string& m) : std::runtime_error(m) {}
};

class LexError : public LatexError {
public:
    explicit LexError(const std::string& m) : LatexError(m) {}
};

class ParseError : public LatexError {
public:
    explicit ParseError(const std::string& m) : LatexError(m) {}
};

class EvalError : public LatexError {
public:
    explicit EvalError(const std::string& m) : LatexError(m) {}
};

} // namespace latexcalc
