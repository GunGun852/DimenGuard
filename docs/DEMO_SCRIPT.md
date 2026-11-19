# One-minute demonstration

Prepare the terminal in the project root. Build and run all tests before the class.
The commands below assume the CMake build on macOS/Linux; replace `build` with
`build-direct` for the compiler-only workflow. On Visual Studio use the matching
`build/Release/*.exe` paths.

| Time | Action | Simple English narration | 繁體中文意思 |
|---|---|---|---|
| 0–10 s | Show the valid formula in `examples/physics.cpp` | "DimenGuard gives physical values strong C++ types. The compiler checks our formulas before they run." | 用型別表示物理單位，執行之前先檢查公式。 |
| 10–25 s | Run `./build/dimenguard demo` | "Distance divided by time becomes speed. Multiplying mass by acceleration gives force. Here the result is four newtons." | 距離除時間得速度，質量乘加速度得力。 |
| 25–40 s | Run the prepared negative compile command below | "Adding metres to seconds is rejected by the actual compiler. This failure is the intended result." | 真正編譯器拒絕米加秒，呢個失敗係預期功能。 |
| 40–50 s | Run `./build/dimenguard convert 1 ft m` | "Feet and metres are both length. An explicit conversion gives zero point three zero four eight metres." | 呎同米都係長度，可以明確轉換。 |
| 50–60 s | Show the explanation output or saved assembly evidence | "The reusable library also explains dimension changes. Our recorded GCC examples match the equivalent raw-double instructions." | 可重用函式庫解釋維度推導；記錄嘅例子同普通 double 指令相同。 |

For GCC/Clang, the intentionally failing command is:

```sh
c++ -std=c++20 -I include -DDIMENGUARD_NEGATIVE_CASE=1 tests/compile_fail/incompatible_add.cpp -o build/invalid_demo
```

The error output is expected. Do not add invalid fixtures directly to the normal
application target. For MSVC use a Developer PowerShell compiler command, or run:

```sh
ctest --test-dir build -C Release -R reject_incompatible_add --verbose
```

That CTest test passes because its valid control compiles and its invalid case
fails. Its compiler log is in `build/compile_cases/incompatible_add/result-ON.log`.

For five presenters, divide the five timed rows among members. The speaking slots
are a rehearsal suggestion, not evidence of who wrote which code.

## Short Q&A preparation

- **Why C++20?** Concepts express which operations are permitted. Integer template
  parameters encode dimensions, and constexpr arithmetic supports constant evaluation.
- **Why not just use doubles?** Bare doubles cannot distinguish length from time in
  a function signature or stop accidental incompatible arithmetic.
- **Why does metre + foot fail?** It is a deliberate explicit-conversion policy;
  the physical dimensions are compatible.
- **Is every conversion free?** No. Runtime scaling can require arithmetic. The
  measured claim is no extra instructions beyond equivalent handwritten arithmetic.
- **Is this new compared with mp-units?** The core technique is established. Our
  teaching emphasis is the structured explanation interface; uniqueness is not proven.
- **What can it not catch?** Wrong external unit labels, invalid physics, double
  rounding/overflow, and same-dimension quantities with different physical meanings.
- **Where is the reusable part?** The public headers and exported CMake target;
  `examples/custom_unit.cpp` extends them without changing the core.
