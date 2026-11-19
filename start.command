#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")"
bash build.sh
if command -v cmake >/dev/null 2>&1; then
    ./build/dimenguard demo
else
    ./build-direct/dimenguard demo
fi
if [[ -t 0 ]]; then read -r -p "Press Enter to close..."; fi
