# 已整合單位全寫的更新包

此包直接提供改好的 C++ 原始碼。更新完成後，每次啟動程式、在 Menu 輸入 `2`，
就會顯示 `s (second)` 等完整名稱。無需再執行 `git apply` 或手動執行 `bash build.sh`。

## 更新一次

此包適用於你截圖中的互動選單版本，包含 yard 和 mile 共 19 種單位。

1. 解壓縮 `DimenGuard_Full_Names_Ready_Update.zip`。
2. 在 GitHub 工作分支上傳以下 5 個檔案，保留原有路徑。
   `app` 內的兩個檔案放在 repository 的 `app` 資料夾，其他檔案放在根目錄。
3. Review、commit，再將 PR 合併到 `main`。
4. 在 Codespaces 同步合併後的 `main`，確保正在使用這個版本。

| 檔案 | 內容 |
|---|---|
| `app/main.cpp` | 已接好 Menu 選項 `2`，直接呼叫完整名稱顯示函式 |
| `app/unit_names.hpp` | 19 種單位的完整名稱 |
| `run.sh` | 自動編譯目前原始碼並開啟選單 |
| `start.command` | 本機 macOS 啟動入口 |
| `START_UPDATE_ZH.md` | 本說明 |

本包包含完整的 `app/main.cpp`，基於先前提供的 19 單位互動版。
若隊友另外修改了該檔案，請在 PR 的 Files changed 中合併及保留那些修改。

建議 commit message：`Integrate full unit names into menu option 2`。

## 之後每次啟動

在 Codespaces 的專案根目錄只執行：

```sh
bash run.sh
```

等待選單出現，輸入：

```text
Choose an option: 2
```

就會得到：

```text
m (metre)
cm (centimetre)
km (kilometre)
ft (foot)
yd (yard)
mi (mile)
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

請固定用 `bash run.sh` 啟動這份專案。它會使用 `build-run` 中的程式；
你先前手動建置的 `build/dimenguard` 可能仍是舊版本。

啟動器會自動處理編譯。有 CMake 時，只重新編譯有變更的部分；
沒有 CMake 時，會用 C++20 編譯器直接編譯 CLI。
兩個流程完成後都直接進入互動選單。

完整名稱功能已寫在本包的 C++ 檔案內，啟動時不依賴任何 `.patch` 檔案。

## 本機 Mac

可以在本機專案資料夾雙擊 `start.command`，啟動後一樣輸入 `2`。
若檔案沒有執行權限，可在本機 Terminal 設定一次 `chmod +x start.command`。
Codespaces 仍使用上述 `bash run.sh`。

## 驗證

2026-11-21，Linux、GCC 14.2.0、CMake 4.4.5：

- 在乾淨的 19 單位專案，直接放入本包的檔案。
- 測試資料夾中沒有任何 `.patch` 檔案，亦沒有執行 `git apply`。
- 只用 `bash run.sh`，首次與第二次啟動的 Menu `2` 都正確列出 19 個完整名稱。
- 第二次啟動重用未變更的執行檔。

本機 macOS Finder 雙擊及 Windows 尚未在這次測試中執行。
