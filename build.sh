#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")"
if command -v cmake >/dev/null 2>&1; then
    cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
    cmake --build build --config Release --parallel 2
    ctest --test-dir build -C Release --output-on-failure --parallel 2
    printf '\nRun the demo: ./build/dimenguard\n'
else
    exec bash tools/build_direct.sh
fi
