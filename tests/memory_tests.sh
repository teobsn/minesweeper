#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
repo_root="$(cd "$script_dir/.." && pwd)"
unit_test_bin="$repo_root/build/minesweeper-unit-tests"
cli_bin="$repo_root/build/minesweeper"

valgrind_cmd=(
    valgrind
    --leak-check=full
    --show-leak-kinds=definite,indirect,possible
    --errors-for-leak-kinds=definite,indirect,possible
    --undef-value-errors=no
    --error-exitcode=1
)

run_checked() {
    local name="$1"
    shift

    local tmp_file
    tmp_file="$(mktemp)"
    if ! timeout 30s "$@" >"$tmp_file" 2>&1; then
        cat "$tmp_file"
        rm -f "$tmp_file"
        echo "[FAIL] $name"
        exit 1
    fi

    rm -f "$tmp_file"
    echo "[OK] $name"
}

if [[ ! -x "$unit_test_bin" ]]; then
    echo "Missing unit test binary: $unit_test_bin"
    exit 1
fi

echo "[1/2] Running unit tests under valgrind"
run_checked "Unit tests leak check" "${valgrind_cmd[@]}" "$unit_test_bin"

if [[ -x "$cli_bin" ]]; then
    echo "[2/2] Running CLI smoke scenarios under valgrind"
    run_checked "CLI win leak check" "${valgrind_cmd[@]}" "$cli_bin" <<< $'2\n1\n1\n2\n1\n3\n0\n6\n1\n\nAlice\n2\n\n5\n'
    run_checked "CLI loss leak check" "${valgrind_cmd[@]}" "$cli_bin" <<< $'2\n3\n1\n1\n1\n2\n1\n6\n1\n\nBob\n2\n\n5\n'
    run_checked "CLI power-up leak check" "${valgrind_cmd[@]}" "$cli_bin" <<< $'1\n12q2\n\n5\n'
else
    echo "[2/2] Skipping CLI leak checks because the binary is missing"
fi

echo "[OK] Memory leak checks passed"
