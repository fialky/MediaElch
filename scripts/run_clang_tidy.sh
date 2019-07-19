#!/usr/bin/env bash

set -euo pipefail
IFS=$'\n\t'

cd "$(dirname "$0")/.."
source scripts/utils.sh

print_important "Building with clang-tidy and applying fixes"

rm -rf build/clang-tidy
mkdir build/clang-tidy
cd build/clang-tidy

print_info "clang-tidy build dir: $(pwd)"

cmake -DCMAKE_BUILD_TYPE=Debug   \
	  -DENABLE_CLANG_TIDY_FIX=ON \
	  -DENABLE_COLOR_OUTPUT=ON   \
	  -GNinja                    \
	  ../..

ninja