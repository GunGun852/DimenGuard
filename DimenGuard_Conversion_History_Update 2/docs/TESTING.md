# Testing and validation record

Validation date: 16 November 2026. Environment: Linux x86-64, GCC 14.2.0,
CMake 4.4.5. Record applies to the delivered source, not every compiler/version.

Interactive-menu update: 20 November 2026, Linux x86-64, GCC 14.2.0,
CMake 4.4.5. The full 17-test CTest suite was rerun; its CLI integration test
now includes the original 16 command-line cases and 12 interactive sessions.
The original sanitizer, install and assembly checks below are historical checks
of the unchanged library, not new platform validation for this console update.

Conversion-history revision: 21 November 2026. The interactive menu now records
successful conversions for the current run and exposes them through option `5`.
The CLI test script adds two sessions for history display and exclusion of failed
conversions (14 interactive sessions total in the updated script). These two new
sessions have not been executed in this update package because it omits the full
project build files, library headers, and test sources. Re-run the complete suite
from the full repository after merging this revision.

## Full suite

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel 2
ctest --test-dir build -C Release --output-on-failure --parallel 2
```

Expected delivery result: **17/17 CTest tests pass**.

| Test area | Evidence |
|---|---|
| Core | 10,043 runtime checks, plus compile-time assertions |
| Numeric sampling | 2,000 deterministic inputs; five comparisons each against independent numerical formulas |
| Type assertions | Deduced velocity/force/area, casts, concepts, conversions, comparisons, scalar rules, layout |
| Compile rejection | 11 separate negative cases, each preceded by a valid compilation control |
| CLI | 16 cases: conversions, incompatible dimensions, malformed/non-finite input, overflow, usage |
| Interactive console | 12 sessions: repeated conversions, menu options, recovery from bad input, whitespace, overflow, EOF at every prompt and explicit interactive mode |
| Applications | Demo, explanations, physics and custom-unit example |
| Integration | Install library, `find_package` from a separate project, build and run consumer |
| Direct build | Compiler-only script builds examples and runs core/negative tests |
| Sanitizers | GCC AddressSanitizer + UndefinedBehaviorSanitizer core run passes |
| Overhead | Three runtime-input typed functions match baseline instructions; see PERFORMANCE.md |

## Negative cases

| File in `tests/compile_fail/` | Rejected operation |
|---|---|
| `incompatible_add.cpp` | Length + time |
| `mixed_scale_add.cpp` | Metres + feet without explicit conversion |
| `wrong_cast.cpp` | Length converted into time |
| `implicit_scalar.cpp` | Quantity implicitly converted to double |
| `implicit_conversion.cpp` | Feet implicitly assigned to metres |
| `scalar_add.cpp` | Bare scalar added to length |
| `incompatible_compare.cpp` | Length compared with time |
| `wrong_argument.cpp` | Mass passed to a duration parameter |
| `invalid_power.cpp` | Power outside documented bounds |
| `invalid_dimension.cpp` | Dimension exponent outside documented bounds |
| `invalid_scale.cpp` | Zero unit scale |

By default each source builds a valid expression. Defining
`DIMENGUARD_NEGATIVE_CASE=1` activates its invalid expression. The harness first
requires the valid control to build, then requires the invalid version to fail.
A missing compiler/header cannot therefore produce a false success. Separate build
directories allow cases to run in parallel. Logs are in
`build/compile_cases/<case>/result-ON.log`.

The harness uses `--clean-first` for each build after changing the negative-case
macro. This prevents reuse of a valid-case object when file timestamps fall in
the same clock tick. The stale-object failure was reproduced locally; a native
macOS rerun is still needed to confirm the reported GitHub Actions failure there.

The explanation demo is not substituted for these actual compiler tests. Compiler
diagnostic wording varies; tests assert acceptance/rejection, not a fragile exact
message string.

## Sanitizer reproduction (GCC/Clang on supported systems)

```sh
cmake -S . -B build-sanitized -DCMAKE_BUILD_TYPE=Debug -DDIMENGUARD_SANITIZERS=ON
cmake --build build-sanitized --target dimenguard_tests
./build-sanitized/dimenguard_tests
```

For this execution environment the equivalent direct GCC run used
`ASAN_OPTIONS=detect_leaks=0`; LeakSanitizer was not evaluated. The core has no
allocation; formatting/diagnostics use standard strings. This is not a proof of
absence of all defects.

## Platform status

Linux GCC was executed here. Native macOS/Apple Clang and Windows/MSVC have not
been executed here. The GitHub Actions matrix is supplied but has not run remotely.
No badge or unexecuted platform is represented as passing. The team should run and
record the actual compiler versions on their own computers before submission.

## Boundaries of the tests

Dimensional checking does not validate physical models or recover the correct unit
of incorrectly labelled external data. Runtime double overflow/NaN semantics remain
those of the numeric representation. There are no GUI, network or browser tests
because this project exposes a C++ library and console application.
