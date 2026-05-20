#!/usr/bin/env bash

set -euo pipefail

export CCACHE_DISABLE=1

build_type="${1:-${BUILD_TYPE:-Debug}}"

cmake -S . -B build -DCMAKE_BUILD_TYPE="$build_type"
cmake --build build
