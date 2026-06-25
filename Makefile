# Convenience shortcuts, powered by uv (https://docs.astral.sh/uv/).
# Run `make help` to list targets.
#
# PYTHON selects the interpreter for the venv. We use a uv-managed standalone
# build by default because it ships the dev headers (Python.h) needed to
# compile the C++ extension. Override like: make dev PYTHON=3.11
PYTHON ?= 3.12

.PHONY: help venv dev build test demo wheel clean

help:
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | \
		awk 'BEGIN {FS = ":.*?## "}; {printf "  \033[36m%-10s\033[0m %s\n", $$1, $$2}'

venv:  ## Create a uv virtual environment in .venv
	uv venv --python $(PYTHON) --managed-python

dev: venv  ## Editable install with dev dependencies (pytest, numpy)
	uv pip install -e ".[dev]"

install: venv  ## Build and install the package
	uv pip install .

build:  ## Rebuild the compiled extension in place (after editing C++)
	uv pip install -e .

test:  ## Run the test suite
	uv run pytest

demo:  ## Run the example script
	uv run python examples/demo.py

wheel:  ## Build a distributable wheel into dist/
	uv build --wheel

clean:  ## Remove build artifacts, the venv, and compiled modules
	rm -rf build dist *.egg-info src/*.egg-info .pytest_cache .mypy_cache .venv
	find . -name '*.so' -delete
	find . -name '__pycache__' -type d -exec rm -rf {} +
