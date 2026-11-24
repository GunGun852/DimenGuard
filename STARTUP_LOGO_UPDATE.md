# DimenGuard — Startup Logo Update

This update adds a plain ASCII DIMENGUARD wordmark above the interactive menu.
Based on GitHub main commit 6861b8547872cef09e20218ad9cdb49bf3faab3b.

## Install once

1. Extract this ZIP.
2. Copy BOTH files from its app folder into your existing project's app folder:
   - app/startup_logo.hpp — new logo artwork and printing function.
   - app/main.cpp — existing main plus one include and one startup call.
3. Commit those two files to your feature branch, review the diff, and merge the PR.
   Suggested commit message: Add ASCII startup logo to interactive menu
4. After your working copy has the merged update, launch normally:

```sh
bash run.sh
```

The existing launcher builds the current source and opens the menu. Install and
commit the update once; no patch needs to be reapplied each time.

If uploading through the GitHub website, open the repository's app directory and
upload main.cpp AND startup_logo.hpp there. Do not upload the ZIP itself as source.

## If your teammate has changed main.cpp since this update

Preserve their changes: add startup_logo.hpp and make only these two insertions
in your CURRENT app/main.cpp instead of replacing the whole file.

Add beside the other application headers:

```cpp
#include "startup_logo.hpp"
```

At the beginning of interactive_menu(), before the welcome message and outside
the while loop, add:

```cpp
dg::cli::print_startup_logo(std::cout);
```

## Behaviour

- The logo appears once per interactive launch, before Welcome to DimenGuard.
- Starting with no arguments or with the interactive argument displays it.
- Returning to the menu does not repeat the logo.
- The existing menu, full unit names and ten-entry conversion history remain.
- Individual CLI commands such as convert and --version keep their existing output.
- The artwork uses ASCII only and is at most 62 columns wide.
- The C++ raw string preserves backslashes without double escaping.

## Verification

Verified on Linux with GCC, C++20 and CMake 4.4.5:
- Existing CTest suite: 17/17 passed.
- Both interactive entry points show the logo once before the menu.
- Menu option 2 includes s (second), yd (yard) and mi (mile).
- Conversion history still records a successful mile-to-yard conversion.
- convert and --version output remains unchanged.
- The CMake launch path opens the logo and menu.

## 繁體中文簡要

把 ZIP 解壓縮後，將 app 資料夾內的兩個檔案放入專案同一個 app 資料夾，
然後 commit 並 merge 一次。之後照常執行 bash run.sh，開啟互動選單時便會
自動出現 Logo，不需要每次套用 patch。請兩個檔案一齊上傳，保留原本路徑。

如果隊友已改過 main.cpp，請保留隊友的內容，只按上方指示加入 include
及 print_startup_logo 那兩行。這個更新包未直接修改你在 GitHub 上的 repository。

