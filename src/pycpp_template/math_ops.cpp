// math_ops.cpp
//
// Implementation of the pure-C++ functions declared in math_ops.hpp.
#include "math_ops.hpp"

#include <stdexcept>

namespace pycpp_template {

std::int64_t add(std::int64_t a, std::int64_t b) {
    return a + b;
}

double dot(const std::vector<double>& a, const std::vector<double>& b) {
    if (a.size() != b.size()) {
        throw std::invalid_argument("dot: input vectors must have equal length");
    }
    double total = 0.0;
    for (std::size_t i = 0; i < a.size(); ++i) {
        total += a[i] * b[i];
    }
    return total;
}

std::vector<double> cumulative_sum(const std::vector<double>& values) {
    std::vector<double> out;
    out.reserve(values.size());
    double running = 0.0;
    for (double v : values) {
        running += v;
        out.push_back(running);
    }
    return out;
}

}  // namespace pycpp_template
