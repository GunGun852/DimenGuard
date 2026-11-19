#!/usr/bin/env bash
# A compiler-only alternative for macOS/Linux. No CMake, Python or downloads.
set -euo pipefail
cd "$(dirname "$0")/.."
compiler="${CXX:-c++}"
mkdir -p build-direct/compile_cases
flags=(-std=c++20 -O2 -Wall -Wextra -Wpedantic -I include)
"$compiler" "${flags[@]}" app/main.cpp -o build-direct/dimenguard
"$compiler" "${flags[@]}" tests/test_core.cpp -o build-direct/dimenguard_tests
"$compiler" "${flags[@]}" examples/physics.cpp -o build-direct/physics_example
"$compiler" "${flags[@]}" examples/custom_unit.cpp -o build-direct/custom_unit_example
./build-direct/dimenguard_tests
for source in tests/compile_fail/*.cpp; do
    case_name="$(basename "$source" .cpp)"
    log="build-direct/compile_cases/$case_name.log"
    "$compiler" "${flags[@]}" "$source" -o "build-direct/compile_cases/$case_name"
    if "$compiler" "${flags[@]}" -DDIMENGUARD_NEGATIVE_CASE=1 "$source" \
        -o "build-direct/compile_cases/$case_name" >"$log" 2>&1; then
        printf 'FAIL: invalid expression compiled: %s\n' "$case_name" >&2
        exit 1
    fi
    printf 'PASS: %s (valid control compiled; invalid expression rejected)\n' "$case_name"
done
./build-direct/physics_example
./build-direct/custom_unit_example
printf '\nRun: ./build-direct/dimenguard\n'
printf 'For the full CLI integration suite and installable CMake package, use CMake.\n'
