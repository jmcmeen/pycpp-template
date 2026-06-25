"""Build configuration for the compiled C++ extension.

Most project metadata lives in ``pyproject.toml``. This file exists only to
declare the C++ extension module, which requires the imperative ``Pybind11Extension``
helper. ``pip install .`` (or ``pip install -e .``) reads both files.
"""

from pybind11.setup_helpers import Pybind11Extension, build_ext
from setuptools import setup

# Every .cpp file that should be compiled into the extension goes here.
SOURCES = [
    "src/pycpp_template/_core.cpp",
    "src/pycpp_template/math_ops.cpp",
]

ext_modules = [
    Pybind11Extension(
        # Import path of the compiled module: ``pycpp_template._core``
        "pycpp_template._core",
        sorted(SOURCES),
        # Make the package version available to C++ as a #define.
        define_macros=[("VERSION_INFO", '"0.1.0"')],
        cxx_std=17,  # C++17; bump to 20 if you need it.
    ),
]

setup(
    ext_modules=ext_modules,
    # build_ext from pybind11 picks the best available C++ standard flags and
    # enables parallel compilation.
    cmdclass={"build_ext": build_ext},
)
