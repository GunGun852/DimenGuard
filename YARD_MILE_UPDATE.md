# Yard / Mile 單位更新

日期：2026-11-21

此更新包適用於已加入互動選單的 DimenGuard 版本
（`DimenGuard_Interactive_Update.zip`）。它包含 8 個更新檔案及本說明，
需要配合原有完整專案使用。

## 新增功能

| 單位 | C++ 型別 | C++ literal | 選單輸入符號 | 定義 |
|---|---|---|---|---|
| yard（碼） | `dimenguard::yard` | `1_yd`、`1.5_yd` | `yd` | 1 yd = 0.9144 m = 3 ft |
| mile（英里） | `dimenguard::mile` | `1_mi`、`0.5_mi` | `mi` | 1 mi = 1609.344 m = 1760 yd |

使用 international yard / mile 定義，比例以 `std::ratio` 儲存。
定義來源：[NIST conversion factors](https://www.nist.gov/pml/special-publication-811/nist-guide-si-appendix-b-conversion-factors)。
實際數值仍使用 `double`，浮點運算可能有捨入誤差。

兩種單位都可以同 `m`、`cm`、`km`、`ft` 互相換算；選單「列出單位」亦會顯示它們。
輸入時使用小寫 `yd`、`mi`。

## 將更新加入 GitHub

1. 先確認準備更新的分支已有互動選單版本，並讓隊友同步該分支。
2. 建立工作分支，例如 `feature/yard-mile`。
3. 解壓縮 ZIP，在 repository 根目錄選擇 **Add file → Upload files**，
   上傳解壓後的檔案及資料夾內容，保留以下相對路徑。
   GitHub 上傳的是原始檔案，而 ZIP 是用來傳送更新包。
4. 檢查 diff：若隊友已修改相同檔案，將雙方修改合併後再提交。
5. 建議 commit message：`Add yard and mile unit support`。
6. 用這個 branch 建立 Pull Request，待測試及 review 完成後合併。

| 路徑 | 更新內容 |
|---|---|
| `include/dimenguard/si.hpp` | 單位型別及 `_yd`、`_mi` literals |
| `include/dimenguard/format.hpp` | `yd`、`mi` 顯示符號 |
| `app/main.cpp` | 選單及 CLI 換算支援 |
| `tests/test_core.cpp` | 數值換算及編譯期型別檢查 |
| `cmake/CliTests.cmake` | CLI 及互動選單測試 |
| `docs/API.md` | 單位及 C++ 使用說明 |
| `docs/TESTING.md` | 更新測試紀錄 |
| `README.md` | 使用例子及測試數量 |
| `YARD_MILE_UPDATE.md` | 本更新說明 |

若在本機套用，將每個檔案放到專案的同名路徑，保留其餘專案檔案。
本更新包不會自動修改 GitHub repository。

## 開啟互動選單

在套用更新的分支及專案根目錄，重新編譯：

```sh
bash build.sh
```

編譯成功後，按照畫面顯示的路徑啟動。使用 CMake 時是：

```sh
./build/dimenguard
```

若電腦沒有 CMake，腳本會使用直接編譯流程；啟動路徑是：

```sh
./build-direct/dimenguard
```

Codespaces 使用 Linux，即使你透過 Mac 瀏覽器開啟，亦可用以上指令。

選擇 `1`（換算），然後逐行輸入：

```text
Enter value: 1
From unit (e.g. m, ft, yd, mi, kg, km/h): mi
To unit: m
1 mi = 1609.344 m
```

| 數值 | 原單位 | 目標單位 | 預期結果 |
|---|---|---|---|
| 1 | yd | ft | 3 ft |
| 1 | yd | m | 0.9144 m |
| 1 | mi | km | 1.609344 km |
| 1 | mi | yd | 1760 yd |
| 1760 | yd | mi | 1 mi |
| 1 | mi | kg | 顯示 incompatible dimensions，並返回選單 |

亦可直接執行：

```sh
./build/dimenguard convert 1 mi yd
```

## C++ 使用例子

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

同一單位可以直接相加。不同長度單位需要明確使用 `unit_cast` 後相加。
`1_mi + 1_s` 或將 mile 轉成 kilogram，會被 C++ 型別限制拒絕。
互動選單的單位由使用者執行時輸入，所以選單的錯誤輸入會在執行時提示。

## 本次測試結果

實測環境：Linux x86-64、GCC 14.2.0、CMake 4.4.5。

- 17/17 CTest 測試通過。
- 10,091 個 runtime core checks 通過，另有編譯期 assertions。
- 37 個 CLI cases 及 14 個互動選單 sessions 通過。
- 原有 11 個「錯誤程式必須編譯失敗」測試全部通過。
- 新增測試涵蓋雙向換算、零、負數、小數、溢位、不相容單位、格式及型別限制。

此次沒有在原生 macOS 或 Windows 執行；提交 PR 後請檢查 GitHub Actions 的結果。
CTest 總數仍是 17，因為新增案例放在原有 `core` 及 `cli_cases` 測試內。

此更新由 AI 協助製作。負責隊友可以在 PR 記錄自己實際 review、修改及測試的內容。
