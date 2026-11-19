# DimenGuard

**Compile-time units and dimensional analysis in C++20.**

DimenGuard is a small, header-only teaching library that gives physical values
strong types. The compiler derives the dimensions of products and quotients and
rejects incompatible addition, comparison, conversion and function arguments.
The optional Dimension Lab explains the exponent arithmetic behind those rules.

**繁體中文安裝及教學：[START_HERE_ZH.md](START_HERE_ZH.md)。**

```cpp
#include <dimenguard/dimenguard.hpp>
using namespace dimenguard::literals;

constexpr auto speed = 100.0_m / 10.0_s;          // 10 m/s
constexpr auto acceleration = speed / 5.0_s;     // 2 m/s²
constexpr auto force = 2.0_kg * acceleration;    // 4 N
static_assert(force.value() == 4.0);

// auto wrong = 1.0_m + 1.0_s;                  // Compile-time error
// auto ambiguous = 1.0_m + 1.0_ft;             // Explicit conversion required
auto distance = 1.0_m + dimenguard::unit_cast<dimenguard::metre>(1.0_ft);
```

## What is included?

- Three base dimensions: length, mass, time; integer exponents are template arguments.
- SI units and selected scaled/non-SI units, including centimetres, feet and hours.
- Automatically derived area, volume, velocity, acceleration and force.
- C++20 concepts, constrained arithmetic, explicit conversion, and integral powers.
- A separate educational API explaining accepted and rejected operations.
- A console demo, typed physics example and custom-unit extension example.
- Eleven real compile-fail cases with valid compilation controls, numerical tests,
  CLI integration checks, and reproducible assembly comparisons.
- Installable CMake interface target, compiler-only build option, and CI configuration.

There is no network service, GUI, database, paid API or third-party library dependency.
Python is optional and used only by the GCC assembly-comparison helper.

## Quick start

Requirements: a C++20-capable compiler; CMake 3.20+ for the standard workflow.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel 2
ctest --test-dir build -C Release --output-on-failure --parallel 2
```

On macOS/Linux:

```sh
./build/dimenguard demo
./build/dimenguard explain
./build/dimenguard convert 72 km/h m/s
./build/physics_example
```

On Windows with the Visual Studio generator, use Developer PowerShell and:

```powershell
.\build\Release\dimenguard.exe demo
.\build\Release\dimenguard.exe explain
.\build\Release\dimenguard.exe convert 72 km/h m/s
```

For Ninja or other single-configuration generators, executables are directly in
`build`, including on Windows. The compiler must be configured before building.

### macOS/Linux without CMake

```sh
bash tools/build_direct.sh
./build-direct/dimenguard demo
```

This compiler-only script runs the core tests and all eleven compile-fail controls.
The full CLI suite and installable package workflow use CMake. `bash build.sh`
chooses CMake when available and otherwise uses this fallback. `bash start.command`
builds, tests and displays the demo; it can also be opened from Terminal on macOS.

## Core design and conversion policy

| Expression | Result |
|---|---|
| `1_m + 2_m` | `3 m` |
| `1_m + 1_s` | Rejected: dimensions differ |
| `1_m + 1_ft` | Rejected: scales differ; convert explicitly |
| `1_m + unit_cast<metre>(1_ft)` | Approximately `1.3048 m` |
| `100_m / 10_s` | `10 m/s` |
| `2_kg * (10_m / 2_s / 5_s)` | `2 N` |
| `unit_cast<one>(1_km / 1_m)` | `1000`, held in a dimensionless quantity |

Metres and feet share a dimension. Requiring a visible `unit_cast` is a deliberate
policy, not a statement that mixed-unit physical calculations are invalid.
Multiplication/division may combine different scales; their resulting unit carries
the combined scale. `.value()` is always expressed in the quantity's own unit.

## Reuse in another C++ project

Copy `include/dimenguard` into an include directory and enable C++20, or use CMake:

```cmake
add_subdirectory(path/to/DimenGuard)
target_link_libraries(my_program PRIVATE DimenGuard::dimenguard)
```

For dependency use, set `DIMENGUARD_BUILD_EXAMPLES=OFF` and
`DIMENGUARD_BUILD_TESTS=OFF` before `add_subdirectory`, or install it:

```sh
cmake --install build --prefix /your/chosen/prefix
```

Then configure the consumer with that prefix in `CMAKE_PREFIX_PATH`:

```cmake
find_package(DimenGuard 1 CONFIG REQUIRED)
target_link_libraries(my_program PRIVATE DimenGuard::dimenguard)
```

## Verified status

The delivered source was built and tested on Linux with GCC 14.2.0:

- 17 CTest tests passed, including eleven negative compilation cases.
- 10,043 runtime core checks passed, plus compile-time assertions.
- 16 CLI scenarios passed inside the CLI integration test.
- ASan/UBSan core run passed with leak detection disabled in this environment.
- Installed-package consumption and the compiler-only workflow passed.
- Three optimized typed functions matched their equivalent raw-double instruction
  sequences under the recorded GCC settings.

Native macOS, Windows and remote GitHub Actions runs have not been performed here.
CI configuration is provided; it is not evidence that those platforms have passed.
See [TESTING.md](docs/TESTING.md) and [PERFORMANCE.md](docs/PERFORMANCE.md).

## Scope and originality

This project is an educational subset, not a replacement for a production units
framework. Values use `double`; temperatures with offsets, fractional powers,
vectors and quantity-kind distinctions are outside this version's scope.

Compile-time dimensional analysis and C++20 concepts are already used by existing
libraries. The reusable explanation API is an educational emphasis, not a verified
claim of uniqueness. Read the honest comparison in [RELATED_WORK.md](docs/RELATED_WORK.md)
before writing the coursework innovation justification.

## Documentation

- [API and limitations](docs/API.md)
- [Architecture and type derivation](docs/ARCHITECTURE.md)
- [Testing and actual results](docs/TESTING.md)
- [Performance evidence](docs/PERFORMANCE.md)
- [One-minute demonstration](docs/DEMO_SCRIPT.md)
- [Suggested five-person work plan](docs/TEAMWORK.md)
- [Submission checklist](docs/SUBMISSION.md)
- [AI development record](docs/AI_USE.md)

License: [MIT](LICENSE).
