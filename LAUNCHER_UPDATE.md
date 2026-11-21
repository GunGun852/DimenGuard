# 一個指令開啟 DimenGuard

日期：2026-11-21

在專案根目錄執行：

```sh
bash run.sh
```

啟動器會自動準備目前分支的程式，然後開啟互動選單。
輸入 `2` 後，已加入單位名稱功能的版本就會顯示：

```text
Choose an option: 2
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

`yd`、`mi` 只會在已加入 yard/mile 功能的版本出現。

## 先將單位全寫功能保存到 GitHub（一次性）

你的截圖顯示 `unit_names_update` 分支套用 patch 後，選單 `2` 已經正常。
`git apply` 會修改工作目錄中的原始碼；需要 commit 及 push，修改才會進入 GitHub 的版本紀錄。

在該分支確認以下兩項修改存在：

- `app/main.cpp` 包含 `#include "unit_names.hpp"`，而 `list_units()` 會呼叫 `print_unit_label`。
- `app/unit_names.hpp` 存在，內有單位的完整名稱。

這兩個檔案必須一同提交。上傳 `.patch` 檔本身不會讓 GitHub 自動修改 C++ 原始碼。

在 Codespaces 可以使用左邊 **Source Control**：

1. 將修改後的 `app/main.cpp` 和 `app/unit_names.hpp` 加入 staged changes。
2. 同時加入本更新包的 `run.sh`、`start.command` 和 `LAUNCHER_UPDATE.md`。
3. 輸入 commit message，例如 `Persist unit names and add simple launcher`。
4. Commit，然後 Push／Publish Branch 到 GitHub。
5. 將該分支的 PR 合併到 `main`，再同步你要使用的 Codespaces。

如果這兩個 C++ 檔案已經提交並合併，只需要新增本更新包的啟動檔案。
其他隊友同步包含這些修改的版本後，亦可直接執行 `bash run.sh`。

Git 的行為說明：
[git apply](https://git-scm.com/docs/git-apply)、
[Codespaces source control](https://docs.github.com/en/codespaces/developing-in-a-codespace/using-source-control-in-your-codespace)。

## 套用這個啟動更新

將更新包的三個檔案放在專案根目錄，即與 `CMakeLists.txt` 同一層：

| 檔案 | 用途 |
|---|---|
| `run.sh` | 自動編譯及開啟選單 |
| `start.command` | macOS 啟動入口，呼叫 `run.sh` |
| `LAUNCHER_UPDATE.md` | 本說明 |

原有 `start.command` 會更新為開啟互動選單。完成後每次只用 `bash run.sh`。
本更新適用於你已完成的互動選單及單位名稱版本；它使用目前分支的原始碼。

## 平時如何使用

```sh
bash run.sh
```

進入選單後輸入 `2`，便會顯示單位名稱。之後可以繼續換算，輸入 `0` 退出。

如要直接列出單位：

```sh
bash run.sh units
```

本機 macOS 使用者亦可開啟 `start.command`。
若檔案經 GitHub 網頁上傳而沒有執行權限，在本機專案根目錄設定一次：

```sh
chmod +x start.command
```

之後可以在 Finder 雙擊 `start.command`。這個方式適用於本機檔案；
Codespaces 的檔案在雲端，使用其 Terminal 的 `bash run.sh` 即可。

## 啟動行為

- 有 CMake：使用 `build-run`，只建置 CLI。CMake 檢查相依檔案，有變更時重新編譯；
  原始碼沒有變更時重用已編譯的程式。
- 沒有 CMake：使用 `c++` 或 `CXX` 指定的 C++20 編譯器，每次直接編譯 CLI 到 `build-run-direct`。
- 編譯失敗時會顯示錯誤並停止，讓你處理失敗的原始碼。
- 完整測試仍可用 `bash build.sh` 執行；平時啟動只準備應用程式。
- 啟動紀錄位於相應建置資料夾的 `launch.log`。

若又看到只顯示 `s` 的列表，請查看目前分支的 `app/main.cpp` 是否已包含
單位名稱修改，並確認你啟動的是這份專案的 `bash run.sh`。
保存到 `main` 的修改，才會跟隨隊友之後下載或同步的版本。

## 本次驗證

Linux、GCC 14.2.0、CMake 4.4.5：

- CMake 首次啟動及再次啟動成功；再次啟動沒有重新編譯未變更的執行檔。
- Menu `2` 顯示單位全寫，`1000 ms = 1 s` 換算及退出流程正常。
- 沒有 CMake 時，直接 C++ 編譯及啟動成功。
- 專案路徑含空格、從其他工作目錄啟動、傳遞 `units` 參數均正常。
- 刻意製造編譯錯誤時，啟動器會停止，沒有啟動先前的舊程式。
- `start.command` 的 Bash 轉接流程在 Linux 驗證；macOS Finder 雙擊尚未實測。

本次只更新啟動流程。C++ 單位名稱版本的 17/17 CTest 結果見前一次更新紀錄。
