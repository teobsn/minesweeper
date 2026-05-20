#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
repo_root="$(cd "$script_dir/.." && pwd)"

unit_test_bin="$repo_root/build/minesweeper-unit-tests"

if [[ ! -x "$unit_test_bin" ]]; then
    echo "Missing unit test binary: $unit_test_bin"
    exit 1
fi

echo "[1/2] Running C++ unit tests"
"$unit_test_bin"

echo "[2/2] Running CLI smoke tests"
"$script_dir/smoke_tests.sh"

echo "[3/3] Running memory leak checks"
"$script_dir/memory_tests.sh"

echo "[OK] All tests passed"
