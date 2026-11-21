# Interactive console update

This update adds a C++ input menu to the existing DimenGuard project. It is an
update package, not a complete standalone project. No change has been pushed to
your GitHub repository automatically.

## Apply the update

1. Extract `DimenGuard_Interactive_Update.zip` into a separate folder.
2. Copy its files into your existing DimenGuard project, matching the paths:
   `app/main.cpp`, `cmake/CliTests.cmake`, `cmake/CompileCase.cmake`, `build.sh`,
   `README.md`, `START_HERE_ZH.md`, `docs/TESTING.md` and this guide.
3. In Codespaces, you can open each matching file, replace its contents with the
   corresponding updated file, and save. Create this guide if it is absent.
   Review any changes your teammates have made before replacing a file.
4. In the terminal at the project root, run `bash build.sh`. After successful
   compilation and tests, run `./build/dimenguard`.

If the build script says it used the compiler-only fallback, run
`./build-direct/dimenguard` instead. On Windows with the Visual Studio generator,
follow the CMake instructions in README and run `.\build\Release\dimenguard.exe`.

You only rebuild after source changes. When you want another conversion, enter
new answers in the same running program. To reopen it later, run the executable.

## Example session

```text
1. Convert units
2. List supported units
3. Show physics demo
4. Explain dimensions
5. Show conversion history
0. Exit

Choose an option: 1
Enter value: 72
From unit (e.g. m, ft, kg, km/h): km/h
To unit: m/s
72 km/h = 20 m/s
```

The menu appears again after each conversion. Choose `5` to review successful
conversions made during the current run; the history is cleared when the program
closes. Choose `0` to exit. Unit symbols are case-sensitive; choose `2` to see
them. Input such as `abc`, an unknown unit, or a conversion from metres to seconds
produces an error and returns to the menu. Failed conversions are not added to
the history.

## What changed

- `app/main.cpp`: reads answers with `std::getline(std::cin, ...)`, uses the same
  conversion implementation as the command-line interface, repeats the menu, and
  records successful conversions for review during the current run. Running without
  arguments now opens this menu. `demo`, `explain`, `units`, `convert`, `--version`
  and `--help` still work.
- `cmake/CliTests.cmake`: contains 14 interactive sessions, including recovery,
  EOF handling, history display, and a check that failed conversions are excluded.
- `cmake/CompileCase.cmake`: includes the earlier `--clean-first` fix so negative
  compilation tests cannot reuse an object from the valid control.
- The build script's final hint and user documentation point to the menu.

The input menu validates user-entered names at runtime. Typed expressions in the
reusable C++ library still enforce dimensions at compile time. The library headers
have not changed in this update.

Suggested commit message after reviewing and testing:
`Add interactive conversion history`.

The two conversion-history test sessions were added in this revision but have not
been executed here because this update package does not include the full project
(build configuration, headers, and test sources). Run `bash build.sh` in the
complete repository before committing.

## 繁體中文快速開始

將 ZIP 入面嘅檔案按原有路徑放返入現有專案；例如 `app/main.cpp`
對應現有嘅 `app/main.cpp`。如果隊友改過同一份檔案，先比較再合併。
呢份係更新包，需要配合原本專案使用。

喺 Codespaces 下方 Terminal 執行 `bash build.sh`，成功後執行
`./build/dimenguard`。揀 `1`，逐項輸入數值、原本單位、目標單位，
每次按 Enter。可以不斷轉換，揀 `0` 離開。下次再開只需執行程式，
改輸入值唔使重新編譯；修改 C++ 原始碼先要重新 build。
