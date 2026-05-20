#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
repo_root="$(cd "$script_dir/.." && pwd)"
cli_bin="$repo_root/build/minesweeper"

if [[ ! -x "$cli_bin" ]]; then
    echo "Missing CLI binary: $cli_bin"
    exit 1
fi

cleanup() {
    if [[ -n "${tmp_win:-}" && -f "$tmp_win" ]]; then
        rm -f "$tmp_win"
    fi
    if [[ -n "${tmp_loss:-}" && -f "$tmp_loss" ]]; then
        rm -f "$tmp_loss"
    fi
    if [[ -n "${tmp_power:-}" && -f "$tmp_power" ]]; then
        rm -f "$tmp_power"
    fi
}

trap cleanup EXIT

run_case() {
    local name="$1"
    local input="$2"
    local expect="$3"
    local tmp_file
    tmp_file="$(mktemp)"
    printf '%s' "$input" | timeout 15s "$cli_bin" >"$tmp_file" 2>&1
    grep -q "$expect" "$tmp_file"
    echo "[OK] $name"
    rm -f "$tmp_file"
}

run_case "CLI win" $'2\n1\n1\n2\n1\n3\n0\n6\n1\n\nAlice\n2\n\n5\n' "You won!"
run_case "CLI loss" $'2\n3\n1\n1\n1\n2\n1\n6\n1\n\nBob\n2\n\n5\n' "You lost! Game over!"
run_case "CLI power-up flow" $'1\n12q2\n\n5\n' "Game quit."
