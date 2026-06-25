# pycpp_template

A complete, batteries-included template for writing **custom Python modules in C++**
using [pybind11](https://pybind11.readthedocs.io/). Clone it, rename one thing, and
start writing fast native code that imports like any normal Python package.

```python
import pycpp_template as cpp

cpp.add(40, 2)                      # 42   (runs in C++)
cpp.dot([1, 2, 3], [4, 5, 6])       # 32.0
acc = cpp.Accumulator()
acc.add(1.5); acc.add(2.5)
acc.total                           # 4.0
```

## Why pybind11?

Writing against the raw CPython C API is verbose and error-prone (manual
reference counting, boilerplate for every function). pybind11 is a header-only
C++11 library that maps C++ functions, classes, exceptions, and STL containers
to Python automatically, while compiling down to a normal native module with no
runtime dependency. It's the most widely used option and needs nothing but a
C++ compiler.

## Requirements

- [uv](https://docs.astral.sh/uv/) — the project's package/environment manager
  (`curl -LsSf https://astral.sh/uv/install.sh | sh`). uv manages Python itself,
  so you don't need a separate Python install.
- A C++17 compiler:
  - **Linux:** `gcc`/`g++` (`sudo apt install build-essential`)
  - **macOS:** Xcode Command Line Tools (`xcode-select --install`)
  - **Windows:** [Build Tools for Visual Studio](https://visualstudio.microsoft.com/visual-cpp-build-tools/)

uv pulls in `pybind11` and `setuptools` automatically at build time — you do not
need to install them yourself.

> **Tip:** use a uv-managed Python (the default here). System Python packages on
> Debian/Ubuntu often lack the dev headers (`Python.h`) required to compile the
> extension; uv's standalone builds include them.

## Quick start

```bash
# Create a venv (uv-managed Python) and do an editable install with dev deps
uv venv --managed-python
uv pip install -e ".[dev]"

# Run the tests
uv run pytest

# Try the demo
uv run python examples/demo.py
```

Or use the Makefile shortcuts: `make dev`, `make test`, `make demo` (`make help`
lists everything). Override the Python version with e.g. `make dev PYTHON=3.11`.

## Project layout

```text
pycpp_template/
├── pyproject.toml              # Build system + metadata (start here)
├── setup.py                    # Declares the C++ extension to compile
├── CMakeLists.txt              # OPTIONAL alternative CMake build
├── Makefile                    # Convenience commands
├── src/pycpp_template/
│   ├── __init__.py             # Python wrapper; re-exports the C++ symbols
│   ├── _core.cpp               # pybind11 bindings (C++ <-> Python glue)
│   ├── math_ops.hpp/.cpp       # Pure C++ logic (no Python dependency)
│   ├── _core.pyi               # Type stubs for editors / mypy
│   └── py.typed                # PEP 561 marker
├── tests/                      # pytest suite
├── examples/demo.py
└── .github/workflows/ci.yml    # Build & test on Linux/macOS/Windows
```

**The key idea:** keep real logic in plain C++ (`math_ops.*`) with no pybind11
includes, and confine all Python-binding code to `_core.cpp`. Your logic stays
reusable and independently testable; the binding file is a thin translation
layer.

## How it fits together

1. `pyproject.toml` declares the build backend (`setuptools`) and build-time
   deps (`pybind11`).
2. `setup.py` lists the `.cpp` files and builds them into the compiled module
   `pycpp_template._core` via `Pybind11Extension`.
3. `_core.cpp`'s `PYBIND11_MODULE(_core, m)` block exposes C++ functions and
   classes to Python. The module name here **must** match the name in
   `setup.py`.
4. `__init__.py` re-exports those symbols so users write
   `from pycpp_template import add`.

## Customizing for your own module

1. **Rename the package.** Replace `pycpp_template` everywhere it appears:
   the `src/pycpp_template/` directory, and the strings in `pyproject.toml`,
   `setup.py`, `_core.cpp` (the `PYBIND11_MODULE` name stays `_core`),
   `__init__.py`, `CMakeLists.txt`, and the tests.

   ```bash
   git grep -l pycpp_template   # find every occurrence
   ```

2. **Add your C++.** Put declarations in a header, implementations in a `.cpp`,
   and add that `.cpp` to the `SOURCES` list in `setup.py`.

3. **Bind it.** Add a `m.def(...)` (functions) or `py::class_<...>` (classes)
   line in `_core.cpp`, then mirror the signature in `_core.pyi`.

4. **Rebuild.** Re-run `uv pip install -e .` (or `make build`). Editable
   installs do **not** auto-recompile C++ — you must rebuild after changing any
   `.cpp`/`.hpp`.

5. **Test.** Add cases under `tests/` and run `uv run pytest`.

## Binding cheat sheet

```cpp
// Function with named, defaulted args
m.def("scale", &scale, py::arg("x"), py::arg("factor") = 2.0);

// A class
py::class_<Widget>(m, "Widget")
    .def(py::init<int>())
    .def("poke", &Widget::poke)
    .def_property_readonly("size", &Widget::size);

// Exceptions: throw a std::exception in C++ -> Python exception.
// std::invalid_argument -> ValueError, std::runtime_error -> RuntimeError.
```

- `#include <pybind11/stl.h>` enables automatic `std::vector`/`map`/etc. ↔
  `list`/`dict` conversion (used here).
- For zero-copy NumPy array access, `#include <pybind11/numpy.h>` and take a
  `py::array_t<double>` argument.
- Release the GIL around heavy C++ loops with
  `py::call_guard<py::gil_scoped_release>()`.

See the [pybind11 docs](https://pybind11.readthedocs.io/) for the full API.

## Building wheels for distribution

For redistributable, manylinux-compatible wheels across many Python versions,
use [`cibuildwheel`](https://cibuildwheel.readthedocs.io/) in CI. Locally,
`make wheel` (`uv build --wheel`) builds a wheel into `dist/` for your current
platform.
