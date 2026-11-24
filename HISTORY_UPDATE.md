# wave2349：本次執行的最近轉換記錄

這是可直接上傳的 C++ 原始碼更新包，基於已有互動選單、yard/mile、
單位全寫及 `run.sh` 的版本。安裝一次後，每次開啟不用套用 patch。

## 新功能

- 選項 **5**：顯示本次執行最近 10 次成功轉換，由舊至新排列。
- 選項 **6**：清除全部記錄，顯示 `Conversion history cleared.`。
- 第 11 次成功轉換會移除最舊一筆，畫面編號維持 1–10。
- 相同的成功轉換會各自記錄；輸入錯誤、未知單位、量綱不相容及數值溢位不會記錄。
- 沒有記錄時顯示 `No successful conversions in this session.`。
- 記錄只存於記憶體，退出／重新開啟程式會清空，不會產生記錄檔。
- 選項 2 繼續顯示 `s (second)` 等 19 個單位全寫。

## wave2349 安裝及提交一次

1. 從團隊最新的 `main` 建立自己的分支，例如 `feature/conversion-history`。
2. 解壓縮 ZIP，把包內檔案放到專案根目錄，保留 `app/`、`cmake/`、`docs/` 路徑。
   若用 GitHub 網頁 Upload files，拖入解壓縮後的內容，確認不是多包一層資料夾。
3. 檢查修改內容，完成以下測試，再以自己的 GitHub 帳戶 commit 及開 PR。
   建議 commit message：`Add session conversion history with clear option`。
4. PR 合併後，其他組員更新到包含這個 commit 的版本，再啟動。

本包的 `app/main.cpp` 是完整檔案。若隊友已在同一檔案加入其他新功能，
請在 PR 的 Files changed 檢查及合併那些修改，避免用較舊檔案覆蓋新功能。
實際測試、理解及修正後再提交，分工紀錄如實描述完成的工作。

## 每次啟動

在專案根目錄的 Terminal 輸入：

```sh
bash run.sh
```

選單會直接出現：

```text
1. Convert units
2. List supported units
3. Show physics demo
4. Explain dimensions
5. Show recent conversions (last 10)
6. Clear conversion history
0. Exit
```

`run.sh` 會編譯目前分支的程式碼並啟動。不需要 `git apply`，也不用另打編譯指令。
更新前若程式仍開著，先選 `0` 退出，再啟動新版本。
直接開舊的 `build/dimenguard` 可能仍是先前編譯的版本，請統一使用 `bash run.sh`。

## 手動驗收

| 操作 | 應有結果 |
|---|---|
| 剛啟動就選 5 | 顯示 0/10，沒有成功記錄 |
| 選 1，依次輸入 `1`、`mi`、`m` | 顯示 `1 mi = 1609.344 m` |
| 再選 1，依次輸入 `72`、`km/h`、`m/s` | 顯示 `72 km/h = 20 m/s` |
| 選 5 | 顯示上述兩筆成功記錄，按操作先後排列 |
| 嘗試 `1 m` 轉成 `s`，再選 5 | 顯示錯誤，仍只有原來兩筆記錄 |
| 選 6，再選 5 | 顯示清除成功，記錄為空 |
| 完成 11 次成功轉換後選 5 | 只有最後 10 次，第一筆已移除 |
| 選 0，重新 `bash run.sh`，選 5 | 新一次執行的記錄為空 |
| 選 2 | 仍顯示 `s (second)`、`yd (yard)`、`mi (mile)` 等全寫 |

顯示例子：

```text
Recent successful conversions (2/10, oldest first):
1. 1 mi = 1609.344 m
2. 72 km/h = 20 m/s
```

## 自動測試及分工說明

執行完整測試：

```sh
bash build.sh
```

CMake 流程會執行全部測試；若機器沒有 CMake，請在 Codespaces 或 CI
使用 CMake 跑完整測試，因為 compiler-only fallback 不會跑互動記錄測試。

本更新已在 Linux、GCC 14.2.0、CMake 4.4.5 驗證：17/17 CTest entries 通過，
內含 37 個 CLI 情境、22 個互動情境，其中 8 個專門驗證記錄功能。
本次沒有執行原生 macOS／Windows 或遠端 GitHub Actions；提交後檢查 PR 的 CI 結果。

核心實作：`app/conversion_history.hpp` 用 `std::deque<std::string>` 保存已成功輸出的結果。
`app/main.cpp` 只在轉換與檢查完成後加入記錄。它屬於互動工具的執行期功能，
不影響 `include/dimenguard/` 的編譯期量綱檢查。

## 包內檔案

| 檔案 | 用途 |
|---|---|
| `app/main.cpp` | 接上記錄功能、選單 5／6 及說明 |
| `app/conversion_history.hpp` | 新增最多 10 筆的記錄容器、顯示及清除 |
| `app/unit_names.hpp` | 附上既有全寫功能，確保相依檔案齊全 |
| `cmake/CliTests.cmake` | 更新選單檢查並載入記錄測試 |
| `cmake/HistoryTests.cmake` | 新增 8 個互動記錄測試情境 |
| `run.sh`、`start.command` | 附上既有簡易啟動器 |
| `README.md`、`docs/TESTING.md` | 更新使用方法及測試紀錄 |
| `HISTORY_UPDATE.md` | 本安裝、驗收及分工指引 |

這是更新包，需要配合原本完整 DimenGuard 專案，不是獨立完整專案。
