#!/usr/bin/env bash
# Build the current source and open the menu with one command.
set -euo pipefail
cd -- "$(dirname "${BASH_SOURCE[0]}")"

show_build_error() {
    printf '\nDimenGuard could not be built. Details:\n' >&2
    cat "$build_log" >&2
    exit 1
}

if command -v cmake >/dev/null 2>&1; then
    build_dir="build-run"
    mkdir -p "$build_dir"
    build_log="$build_dir/launch.log"
    printf 'Preparing DimenGuard...\n' >&2
    if ! cmake -S . -B "$build_dir" -DCMAKE_BUILD_TYPE=Release \
        -DDIMENGUARD_BUILD_EXAMPLES=ON -DDIMENGUARD_BUILD_TESTS=OFF \
        >"$build_log" 2>&1; then
        show_build_error
    fi
    if ! cmake --build "$build_dir" --config Release \
        --target dimenguard_cli --parallel 2 >>"$build_log" 2>&1; then
        show_build_error
    fi
    # Support single-configuration and multi-configuration CMake generators.
    for program in "$build_dir/Release/dimenguard" \
        "$build_dir/Release/dimenguard.exe" \
        "$build_dir/dimenguard" "$build_dir/dimenguard.exe"; do
        if [[ -x "$program" ]]; then
            exec "$program" "$@"
        fi
    done
    printf 'Build finished, but the DimenGuard executable was not found. See %s.\n' \
        "$build_log" >&2
    exit 1
else
    compiler="${CXX:-c++}"
    if ! command -v "$compiler" >/dev/null 2>&1; then
        printf 'A C++20 compiler is required. Install one or set CXX to its executable path.\n' >&2
        exit 1
    fi
    build_dir="build-run-direct"
    mkdir -p "$build_dir"
    build_log="$build_dir/launch.log"
    printf 'Compiling DimenGuard...\n' >&2
    if ! "$compiler" -std=c++20 -O2 -Wall -Wextra -Wpedantic \
        -I include app/main.cpp -o "$build_dir/dimenguard" \
        >"$build_log" 2>&1; then
        show_build_error
    fi
    exec "$build_dir/dimenguard" "$@"
fi
