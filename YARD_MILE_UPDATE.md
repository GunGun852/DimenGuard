# Yard / Mile Unit Update

Date: 2026-11-21

This update package is intended for the version of DimenGuard that already includes the interactive menu (`DimenGuard_Interactive_Update.zip`). It contains eight updated files and this guide, and must be used together with the existing complete project.

## New Features

| Unit | C++ type | C++ literal | Menu input symbol | Definition |
|---|---|---|---|---|
| Yard | `dimenguard::yard` | `1_yd`, `1.5_yd` | `yd` | 1 yd = 0.9144 m = 3 ft |
| Mile | `dimenguard::mile` | `1_mi`, `0.5_mi` | `mi` | 1 mi = 1609.344 m = 1760 yd |

The units follow the international yard and mile definitions, with conversion ratios stored using `std::ratio`.

Definition source: [NIST conversion factors](https://www.nist.gov/pml/special-publication-811/nist-guide-si-appendix-b-conversion-factors).

Actual numerical values still use `double`, so floating-point calculations may involve rounding errors.

Both units support conversion to and from `m`, `cm`, `km` and `ft`. They also appear in the menu’s unit list.

Use lowercase `yd` and `mi` when entering unit symbols.

## Adding the Update to GitHub

1. Confirm that the target branch already contains the interactive menu version, and ask your teammates to synchronise their copies with that branch.
2. Create a working branch, such as `feature/yard-mile`.
3. Extract the ZIP file. At the repository root, select **Add file → Upload files**, then upload the extracted files and folder contents while preserving the relative paths listed below. Upload the source files to GitHub; the ZIP is used to distribute the update package.
4. Review the diff. If a teammate has modified the same files, combine both sets of changes before committing.
5. Suggested commit message: `Add yard and mile unit support`.
6. Create a Pull Request from this branch and merge it after testing and review are complete.

| Path | Changes |
|---|---|
| `include/dimenguard/si.hpp` | Unit types and `_yd` / `_mi` literals |
| `include/dimenguard/format.hpp` | Display symbols for `yd` and `mi` |
| `app/main.cpp` | Conversion support in the interactive menu and CLI |
| `tests/test_core.cpp` | Numerical conversion tests and compile-time type checks |
| `cmake/CliTests.cmake` | CLI and interactive menu tests |
| `docs/API.md` | Unit documentation and C++ usage instructions |
| `docs/TESTING.md` | Updated testing records |
| `README.md` | Usage examples and test counts |
| `YARD_MILE_UPDATE.md` | This update guide |

When applying the update locally, place each file at its corresponding project path and retain the other project files.

This update package does not automatically modify the GitHub repository.

## Opening the Interactive Menu

Switch to the branch containing the update, open the project’s root directory, and rebuild:

```sh
bash build.sh
```

After compilation succeeds, launch the program using the path shown in the terminal. When using CMake, run:

```sh
./build/dimenguard
```

If CMake is unavailable, the script uses the direct compilation workflow. In that case, run:

```sh
./build-direct/dimenguard
```

Codespaces runs Linux, even when accessed through a browser on a Mac, so the commands above also apply.

Select `1` to convert units, then enter the following values one line at a time:

```text
Enter value: 1
From unit (e.g. m, ft, yd, mi, kg, km/h): mi
To unit: m
1 mi = 1609.344 m
```

| Value | Source unit | Target unit | Expected result |
|---|---|---|---|
| 1 | yd | ft | 3 ft |
| 1 | yd | m | 0.9144 m |
| 1 | mi | km | 1.609344 km |
| 1 | mi | yd | 1760 yd |
| 1760 | yd | mi | 1 mi |
| 1 | mi | kg | Displays an “incompatible dimensions” error and returns to the menu |

You can also run a conversion directly:

```sh
./build/dimenguard convert 1 mi yd
```

## C++ Usage Example

```cpp
#include <dimenguard/dimenguard.hpp>
#include <iostream>

int main() {
    namespace dg = dimenguard;
    using namespace dg::literals;

    std::cout << dg::unit_cast<dg::metre>(1_mi) << '\n';
    std::cout << dg::unit_cast<dg::foot>(1_yd) << '\n';
    auto total = 1_yd + dg::unit_cast<dg::yard>(3_ft);
    std::cout << total << '\n'; // 2 yd
}
```

Quantities with the same unit can be added directly. Different length units require explicit conversion using `unit_cast` before addition.

Expressions such as `1_mi + 1_s`, or attempts to convert miles into kilograms, are rejected by the C++ type constraints.

In the interactive menu, users enter unit symbols at runtime, so invalid menu input produces a runtime error message.

## Test Results for This Update

Test environment: Linux x86-64, GCC 14.2.0 and CMake 4.4.5.

- All 17 CTest tests passed.
- All 10,091 runtime core checks passed, alongside compile-time assertions.
- All 37 CLI cases and 14 interactive menu sessions passed.
- All 11 existing tests requiring invalid programs to fail compilation passed.
- New tests cover conversions in both directions, zero, negative values, decimals, overflow, incompatible units, formatting and type constraints.

This update was not tested on native macOS or Windows. Check the GitHub Actions results after submitting the Pull Request.

The total number of CTest entries remains 17 because the additional cases are included within the existing `core` and `cli_cases` tests.

This update was developed with AI assistance. The responsible team member can document the review, modifications and testing they actually performed in the Pull Request.
