# Full Unit Name Display Update

Update date: 2026-11-21

The `2. List supported units` menu option and the `dimenguard units` command now display units in the format `s (second)`.

```text
m (metre)
cm (centimetre)
km (kilometre)
ft (foot)
kg (kilogram)
g (gram)
s (second)
ms (millisecond)
min (minute)
h (hour)
m^2 (square metre)
m^3 (cubic metre)
m/s (metre per second)
km/h (kilometre per hour)
m/s^2 (metre per second squared)
N (newton)
1 (dimensionless)
```

If your version already supports yards and miles, the list will also include `yd (yard)` and `mi (mile)`.

The list is still generated from the units actually supported by the project. This update only adds display names.

Continue entering symbols such as `s` and `m` when performing conversions, for example: `1000 ms = 1 s`.

## Update Package Contents

| File | Purpose |
|---|---|
| `app/unit_names.hpp` | C++ unit names and display functions |
| `unit_names.patch` | Modifies two locations in the existing `app/main.cpp` |
| `UNIT_NAMES_UPDATE.md` | This update guide |

This package is intended for DimenGuard versions that already include the interactive menu. It has been validated with both the 17-unit version and the 19-unit version containing yards and miles.

A targeted patch is used to preserve other changes to the program.

## Method A: Apply the Update in Codespaces

1. Synchronise your local copy with the team’s latest `main` branch, then create a working branch, such as `feature/unit-full-names`. If you have uncommitted changes, save and resolve them first.
2. Extract the update package and place the three files in the project, preserving the paths shown above. Put `unit_names.hpp` inside the existing `app` folder. Put the other two files in the project root, at the same level as `CMakeLists.txt`.
3. In the terminal at the project root, run the following commands **one line at a time**, confirming that each step succeeds before continuing.

```sh
git apply --check unit_names.patch
git apply unit_names.patch
bash build.sh
```

The first command usually produces no output when successful. The second command actually updates `app/main.cpp`.

If you see `patch failed`, the relevant code may have been modified, or this update may already have been applied. Use Method B below to check the two locations.

4. After compilation succeeds, launch the program using the path shown in the terminal. For the CMake version, run:

```sh
./build/dimenguard
```

If the script indicates that it used direct compilation, run:

```sh
./build-direct/dimenguard
```

5. Select `2` and confirm that `s (second)` appears. Then select `1` and enter `1000`, `ms` and `s`. Confirm that the result is `1000 ms = 1 s`.
6. Review and commit the changes. Suggested commit message: `Show full unit names in the supported units list`. Then create a Pull Request.

The files required for this feature are `app/main.cpp` and `app/unit_names.hpp`. Include both files in the same Pull Request.

You may also retain this guide and the patch as records of the update.

## Method B: Edit Directly on GitHub

If you only use the GitHub website, make the following two changes without running the patch.

1. Add `app/unit_names.hpp` to your working branch, using the contents of the file with the same name in the update package.
2. Edit `app/main.cpp` on that branch, changing only the two locations below.

Add the new include directly below the existing DimenGuard include at the top of the file:

```cpp
#include <dimenguard/dimenguard.hpp>
#include "unit_names.hpp"
```

Replace the existing `list_units()` function with:

```cpp
void list_units() {
    for (const auto& entry : registry) {
        dg::cli::print_unit_label(std::cout, entry.symbol);
        std::cout << '\n';
    }
}
```

Keep the rest of the program unchanged and submit a Pull Request. Afterwards, synchronise that branch in Codespaces, rebuild the project and test it.

## Validation Record

- Environment: Linux x86-64, GCC 14.2.0 and CMake 4.4.5.
- **19-unit version:** All 17 existing CTest tests passed.
- **17-unit interactive version:** Successfully compiled with C++20 after applying the patch.
- For both versions, the `units` output and the interactive sequence “list units → convert → exit” were checked.
- The patch was successfully applied to the original `app/main.cpp` in both versions.
- Native macOS and Windows were not tested during this validation. Check the CI results for your Pull Request.

The number of existing CTest entries remains 17. This display update was additionally verified by checking the actual console output.

This update package does not automatically upload or merge changes on GitHub.
