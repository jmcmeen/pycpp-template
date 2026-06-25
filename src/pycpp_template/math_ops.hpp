// math_ops.hpp
//
// Pure C++ declarations with no Python/pybind11 dependency. Keeping your real
// logic free of binding code makes it independently testable and reusable.
#pragma once

#include <cstdint>
#include <vector>

namespace pycpp_template {

// Add two integers.
std::int64_t add(std::int64_t a, std::int64_t b);

// Dot product of two equal-length vectors. Throws std::invalid_argument if the
// sizes differ (pybind11 translates this into a Python ValueError).
double dot(const std::vector<double>& a, const std::vector<double>& b);

// Return the running cumulative sum of the input.
std::vector<double> cumulative_sum(const std::vector<double>& values);

}  // namespace pycpp_template
