# Session Conversion History Update

This package contains C++ source updates ready to upload. It is based on the version that already includes the interactive menu, yard/mile units, full unit names and `run.sh`.

Apply the update once. You do not need to apply a patch each time you launch the program.

## New Features

- Option **5** displays the last 10 successful conversions from the current session, ordered from oldest to newest.
- Option **6** clears all records and displays `Conversion history cleared.`
- The 11th successful conversion removes the oldest record. Displayed entries remain numbered 1–10.
- Repeated successful conversions are recorded separately. Invalid input, unknown units, incompatible dimensions and numerical overflow are not recorded.
- When the history is empty, the program displays `No successful conversions in this session.`
- Records are stored only in memory. Exiting or restarting the program clears them, and no history file is created.
- Option **2** continues to display all 19 units with their full names, such as `s (second)`.

## One-Time Installation and Submission

1. Create a working branch from the latest `main`, such as `feature/conversion-history`.
2. Extract the ZIP and place its contents in the project root, preserving the `app/`, `cmake/` and `docs/` paths. If using GitHub’s **Upload files** page, drag in the extracted contents and ensure that no extra enclosing folder is added.
3. Review the changes, complete the tests below, then commit and open a Pull Request. Suggested commit message: `Add session conversion history with clear option`.
4. After the Pull Request is merged, update your working copy to a version containing this commit before launching the program.

The package includes a complete replacement for `app/main.cpp`. If other features have since been added to that file, review and combine the changes in the Pull Request’s **Files changed** tab to avoid overwriting newer features with an older file.

Understand, test and correct the changes before submitting them.

## Launching the Program

From the terminal in the project root, run:

```sh
bash run.sh
```

The interactive menu will appear:

```text
1. Convert units
2. List supported units
3. Show physics demo
4. Explain dimensions
5. Show recent conversions (last 10)
6. Clear conversion history
0. Exit
```

`run.sh` compiles the code from the current branch and launches the program. There is no need to run `git apply` or enter a separate compilation command.

If the program is still running when you update it, select `0` to exit before launching the new version.

Running an existing `build/dimenguard` executable directly may launch a previously compiled version. Use `bash run.sh` consistently to build and launch the current source.

## Manual Acceptance Tests

| Action | Expected result |
|---|---|
| Select `5` immediately after launching | Displays 0/10 with no successful conversions |
| Select `1`, then enter `1`, `mi` and `m` | Displays `1 mi = 1609.344 m` |
| Select `1` again, then enter `72`, `km/h` and `m/s` | Displays `72 km/h = 20 m/s` |
| Select `5` | Displays the two successful conversions in chronological order |
| Attempt to convert `1 m` to `s`, then select `5` | Displays an error; the history still contains only the original two records |
| Select `6`, then select `5` | Confirms that the history was cleared and shows an empty history |
| Complete 11 successful conversions, then select `5` | Displays only the last 10 conversions; the first has been removed |
| Select `0`, run `bash run.sh` again, then select `5` | The new session starts with an empty history |
| Select `2` | Still displays full names, including `s (second)`, `yd (yard)` and `mi (mile)` |

Example output:

```text
Recent successful conversions (2/10, oldest first):
1. 1 mi = 1609.344 m
2. 72 km/h = 20 m/s
```

## Automated Testing and Implementation

Run the full test suite:

```sh
bash build.sh
```

The CMake workflow runs all tests. If CMake is unavailable on your machine, run the full suite using CMake in Codespaces or CI, because the compiler-only fallback does not run the interactive history tests.

This update was validated on Linux with GCC 14.2.0 and CMake 4.4.5. All **17/17 CTest entries passed**, including 37 CLI scenarios and 22 interactive scenarios. Eight of the interactive scenarios specifically test the history feature.

Native macOS, Windows and remote GitHub Actions were not run during this validation. Check the Pull Request’s CI results after submission.

The core implementation is in `app/conversion_history.hpp`, which uses `std::deque<std::string>` to store successfully displayed conversion results.

`app/main.cpp` adds a record only after the conversion and validation checks have completed successfully. This is a runtime feature of the interactive application and does not affect the compile-time dimensional checks in `include/dimenguard/`.

## Package Contents

| File | Purpose |
|---|---|
| `app/main.cpp` | Integrates conversion history, menu options 5 and 6, and help text |
| `app/conversion_history.hpp` | Adds a history container with a maximum of 10 entries, plus display and clear functions |
| `app/unit_names.hpp` | Includes the existing full-name display feature to ensure the required dependencies are present |
| `cmake/CliTests.cmake` | Updates menu checks and loads the history tests |
| `cmake/HistoryTests.cmake` | Adds eight interactive history test scenarios |
| `run.sh`, `start.command` | Includes the existing simplified launchers |
| `README.md`, `docs/TESTING.md` | Updates usage instructions and testing records |
| `HISTORY_UPDATE.md` | This installation, acceptance testing and implementation guide |

This is an update package. It must be used with the existing complete DimenGuard project and is not a standalone project.
