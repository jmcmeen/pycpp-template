"""pycpp_template — a template Python package backed by a C++ extension.

The compiled C++ lives in the ``_core`` submodule. We re-export its public
names here so users can write ``from pycpp_template import add`` and never need
to know there is a binary module underneath. This wrapper is also the right
place to add pure-Python convenience code on top of the fast C++ core.
"""

from ._core import Accumulator, add, cumulative_sum, dot
from ._core import __version__

__all__ = [
    "add",
    "dot",
    "cumulative_sum",
    "Accumulator",
    "__version__",
]
