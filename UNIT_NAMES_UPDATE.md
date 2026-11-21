# 單位全寫顯示更新

更新日期：2026-11-21

選單 `2. List supported units` 及 `dimenguard units` 現在會以
`s (second)` 的形式顯示單位。

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

如果你的版本已支援 yard 和 mile，列表亦會顯示 `yd (yard)`、`mi (mile)`。
列表仍根據專案實際支援的單位產生；這個更新只加入顯示名稱。
換算時繼續輸入 `s`、`m` 等符號，例如 `1000 ms = 1 s`。

## 更新包內容

| 檔案 | 用途 |
|---|---|
| `app/unit_names.hpp` | C++ 單位名稱及顯示函式 |
| `unit_names.patch` | 修改現有 `app/main.cpp` 的兩處位置 |
| `UNIT_NAMES_UPDATE.md` | 本說明 |

此包適用於已有互動選單的 DimenGuard，已分別驗證 17 種單位版本及
加入 yard/mile 後的 19 種單位版本。使用局部 patch 保留其餘程式修改。

## 方法 A：在 Codespaces 套用

1. 先同步你們的最新 `main`，再建立工作分支，例如 `feature/unit-full-names`。
   若有未提交的改動，先保存及處理好該改動。
2. 解壓縮更新包，將三個檔案放入專案，按上表保留路徑。
   `unit_names.hpp` 放在原有 `app` 資料夾，另外兩個檔案放在專案根目錄
   （與 `CMakeLists.txt` 同一層）。
3. 在專案根目錄的 Terminal **逐行**執行以下指令，確認每一步成功再繼續。

```sh
git apply --check unit_names.patch
git apply unit_names.patch
bash build.sh
```

第一行成功通常不會顯示文字。第二行才會實際更新 `app/main.cpp`。
如果出現 `patch failed`，代表對應程式已有其他修改，或已套用過此更新；
請使用下面方法 B 核對兩個位置。

4. 編譯成功後按畫面提示啟動；使用 CMake 的版本是：

```sh
./build/dimenguard
```

若腳本提示直接編譯版本，則使用：

```sh
./build-direct/dimenguard
```

5. 選擇 `2`，確認出現 `s (second)`。接着選擇 `1`，輸入
   `1000`、`ms`、`s`，確認得到 `1000 ms = 1 s`。
6. Review 並 commit 功能改動，建議訊息：
   `Show full unit names in the supported units list`，然後建立 PR。

功能所需檔案是 `app/main.cpp` 和 `app/unit_names.hpp`；
請讓兩個檔案一同進入 PR。本說明及 patch 可一併保留作更新紀錄。

## 方法 B：直接在 GitHub 網頁編輯

如果你只使用 GitHub 網頁，可以完成以下兩項修改，毋須執行 patch。

1. 在工作分支新增 `app/unit_names.hpp`，內容使用更新包內的同名檔案。
2. 編輯該分支的 `app/main.cpp`，只改以下兩個位置。

在檔案頂部原有的 include 下加入一行：

```cpp
#include <dimenguard/dimenguard.hpp>
#include "unit_names.hpp"
```

將原有的 `list_units()` 函式改為：

```cpp
void list_units() {
    for (const auto& entry : registry) {
        dg::cli::print_unit_label(std::cout, entry.symbol);
        std::cout << '\n';
    }
}
```

保留其餘程式內容，再提交 PR。之後在 Codespaces 同步該分支、重新編譯及測試。

## 驗證紀錄

- Linux x86-64，GCC 14.2.0，CMake 4.4.5。
- 19 種單位版本：現有 17/17 CTest 測試通過。
- 17 種單位互動版本：套用 patch 後，以 C++20 編譯成功。
- 兩個版本均實際檢查 `units` 輸出，以及「選單列出單位 → 換算 → 退出」。
- patch 已在兩個版本的原始 `app/main.cpp` 上成功套用。
- 原生 macOS 和 Windows 未在這次驗證中執行；請檢查你們 PR 的 CI 結果。

現有 17 項 CTest 測試數量不變。本次顯示更新另以實際 console 輸出核對。
這個更新包不會自動上傳或合併 GitHub 上的改動。
