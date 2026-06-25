"""Minimal demo of the pycpp_template C++ extension.

Run after installing the package:

    pip install -e .
    python examples/demo.py
"""

import pycpp_template as cpp


def main() -> None:
    print(f"pycpp_template version: {cpp.__version__}")

    print("add(40, 2) =", cpp.add(40, 2))
    print("dot([1,2,3], [4,5,6]) =", cpp.dot([1.0, 2.0, 3.0], [4.0, 5.0, 6.0]))
    print("cumulative_sum([1,2,3,4]) =", cpp.cumulative_sum([1.0, 2.0, 3.0, 4.0]))

    acc = cpp.Accumulator()
    for value in (1.5, 2.5, 3.0):
        acc.add(value)
    print("accumulator total:", acc.total)
    print("accumulator repr:", repr(acc))


if __name__ == "__main__":
    main()
