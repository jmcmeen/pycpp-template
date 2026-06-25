// _core.cpp
//
// The binding layer: this is the only file that knows about both C++ and
// Python. It exposes the pure-C++ functions from math_ops.hpp to Python via
// pybind11. The compiled result is importable as ``pycpp_template._core``.

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>  // automatic std::vector <-> list conversion

#include "math_ops.hpp"

namespace py = pybind11;

#ifndef VERSION_INFO
#define VERSION_INFO "dev"
#endif

// A small example of a C++ class exposed to Python, to show stateful objects.
namespace {
class Accumulator {
public:
    explicit Accumulator(double start = 0.0) : total_(start) {}

    double add(double value) {
        total_ += value;
        return total_;
    }

    double total() const { return total_; }

    void reset(double start = 0.0) { total_ = start; }

private:
    double total_;
};
}  // namespace

// PYBIND11_MODULE defines the entry point. The first argument MUST match the
// module name in setup.py (here: ``_core``).
PYBIND11_MODULE(_core, m) {
    m.doc() = "C++ core for pycpp_template (compiled with pybind11)";

    // --- Free functions ----------------------------------------------------
    m.def("add", &pycpp_template::add, py::arg("a"), py::arg("b"),
          "Add two integers and return the result.");

    m.def("dot", &pycpp_template::dot, py::arg("a"), py::arg("b"),
          "Return the dot product of two equal-length sequences of floats.");

    m.def("cumulative_sum", &pycpp_template::cumulative_sum, py::arg("values"),
          "Return the running cumulative sum of a sequence of floats.");

    // --- Example class -----------------------------------------------------
    py::class_<Accumulator>(m, "Accumulator",
                            "A running total you can keep adding to.")
        .def(py::init<double>(), py::arg("start") = 0.0)
        .def("add", &Accumulator::add, py::arg("value"),
             "Add a value and return the new total.")
        .def("reset", &Accumulator::reset, py::arg("start") = 0.0,
             "Reset the running total.")
        .def_property_readonly("total", &Accumulator::total,
                               "The current running total.")
        .def("__repr__", [](const Accumulator& a) {
            return "<Accumulator total=" + std::to_string(a.total()) + ">";
        });

    m.attr("__version__") = VERSION_INFO;
}
