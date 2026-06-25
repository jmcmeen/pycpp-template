# CMakeLists.txt — OPTIONAL alternative build path.
#
# The default build uses setuptools + pyproject.toml (just run `pip install .`).
# This file is only needed if you prefer a CMake-driven workflow, e.g. building
# with scikit-build-core or compiling the extension directly with CMake for
# debugging in an IDE.
#
# Direct CMake build (produces a _core*.so next to the sources):
#   cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
#   cmake --build build
#
cmake_minimum_required(VERSION 3.15)
project(pycpp_template LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Locate Python and pybind11. With pybind11 installed via pip, point CMake at
# its cmake config:  -Dpybind11_DIR=$(python -m pybind11 --cmakedir)
find_package(Python COMPONENTS Interpreter Development.Module REQUIRED)
find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(_core
    src/pycpp_template/_core.cpp
    src/pycpp_template/math_ops.cpp
)

target_compile_definitions(_core PRIVATE VERSION_INFO="0.1.0")

# Place the built module inside the package directory so it is importable.
set_target_properties(_core PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/src/pycpp_template"
)
