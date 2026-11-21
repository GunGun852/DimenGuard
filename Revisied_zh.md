# DimenGuard：從這裡開始

本專案是一個 **C++20 編譯期單位與維度檢查函式庫**。它的目標是讓數值具有對應的單位型別，令編譯器能在程式執行前偵測指定類型的錯誤。

例如，米與秒不能直接相加，相關程式碼會在編譯時失敗；距離除以時間則會自動推導出速度型別。

專案核心是一個 library，並附有可直接執行的示範程式。

## 1. 先編譯，再啟動互動選單

解壓縮 ZIP 檔案，使用 VS Code 開啟 `DimenGuard` 資料夾，然後開啟 Terminal。請先確認 Terminal 所在位置是包含 `README.md` 和 `CMakeLists.txt` 的專案根目錄。如果你已經使用 Codespaces，直接使用畫面下方的 Terminal 即可。

第一次編譯，或修改 C++ 原始碼後，請執行：

```sh
bash build.sh
```

編譯及測試成功後，啟動程式：

```sh
./build/dimenguard
```

如果電腦沒有安裝 CMake，build script 會改用直接編譯流程，並在完成後提示你執行 `./build-direct/dimenguard`。之後如只更改輸入數值，便無須重新編譯，直接重新啟動程式即可。

在選單中選擇 `1` 進行單位換算，並按照提示逐項輸入資料；每輸入一項，按 Enter 繼續：

```text
Choose an option: 1
Enter value: 72
From unit (e.g. m, ft, kg, km/h): km/h
To unit: m/s
72 km/h = 20 m/s
```

完成換算後，程式會自動返回主選單，讓你繼續操作。其他選項如下：

- `2`：列出所有支援的單位。單位符號區分大小寫，例如 `m` 和 `N` 代表不同單位。
- `3`：顯示物理計算示範。
- `4`：顯示維度推導說明。
- `0`：離開程式。

如果輸入無效選項或不相容的單位，例如嘗試將米換算成秒，程式會顯示錯誤提示，然後返回主選單。

在 Mac 上，如果你只想使用 compiler，也可以執行：

```sh
bash tools/build_direct.sh
./build-direct/dimenguard
```

這個方法只需要支援 C++20 的 compiler。如果尚未安裝 Apple 開發工具，可以先執行：

```sh
xcode-select --install
```

完成安裝後，再執行 build 指令。如果使用的 compiler 版本較舊，未能支援 C++20，便需要更新開發工具。

目前已在 Linux/GCC 環境驗證程式；至於 macOS 本機環境，仍需自行執行測試確認。

在選單中選擇 `3`，即可查看物理計算示範，例如：

```text
100 m / 10 s         = 10 m/s
(100 m / 10 s) / 5 s = 2 m/s^2
2 kg * acceleration  = 4 N
1 m + cast<m>(1 ft)  = 1.3048 m
72 km/h in m/s       = 20 m/s
```

## 2. 查看維度推導

執行以下指令：

```sh
./build-direct/dimenguard explain
```

`L` 代表長度（Length），`M` 代表質量（Mass），`T` 代表時間（Time）。本專案使用 `^` 表示指數：

| 物理量 | L | M | T | 解釋 |
|---|---:|---:|---:|---|
| 距離 | 1 | 0 | 0 | 長度 |
| 時間 | 0 | 0 | 1 | 時間 |
| 速度 | 1 | 0 | -1 | 長度／時間 |
| 加速度 | 1 | 0 | -2 | 長度／時間² |
| 力 | 1 | 1 | -2 | 質量 × 加速度 |

乘法會將指數相加，除法則會將指數相減。

例如，速度除以時間時：`(1,0,-1) - (0,0,1) = (1,0,-2)`，結果就是加速度。

`explain` 會根據型別資料說明運算過程，但不會真的將非法加法寫進主程式。若要查看編譯器拒絕錯誤運算的證據，可以參考 `tests/compile_fail` 及相關自動測試。

## 3. 單位換算

你可以在互動選單中選擇 `1`，按照提示輸入換算資料。程式亦支援以下命令列指令：

```sh
./build-direct/dimenguard convert 1 ft m
./build-direct/dimenguard convert 500 g kg
./build-direct/dimenguard convert 72 km/h m/s
./build-direct/dimenguard units
```

如果使用 CMake 編譯，請將以上指令中的 `build-direct` 路徑改為 `build`。

米和呎都屬於長度單位，因此可以互相換算。不過，這個 library 要求先使用 `unit_cast` 明確轉換單位，才可以將數值相加：

```cpp
auto length = 1_m + dimenguard::unit_cast<dimenguard::metre>(1_ft);
```

互動選單使用 `std::getline(std::cin, ...)` 讀取輸入。由於單位名稱是在程式執行時由使用者輸入，因此無效的單位名稱會在執行時檢查；至於 C++ 原始碼中的單位型別及運算公式，則會在編譯時由編譯器檢查。這兩種檢查方式需要清楚區分。

## 4. 使用 CMake 執行完整測試

如果電腦已安裝 CMake 3.20 或以上版本，可以執行以下指令：

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel 2
ctest --test-dir build -C Release --output-on-failure --parallel 2
./build/dimenguard demo
```

使用 CMake 編譯時，程式位於 `build` 資料夾；使用 compiler-only script 時，程式則位於 `build-direct` 資料夾。

VS Code 亦提供 configure、build、test 和 demo tasks，可直接在編輯器中執行相關工作。

Windows 使用者可以在已設定 C++ 開發環境的 Developer PowerShell 中執行以上 CMake 指令。使用 Visual Studio generator 時，程式通常位於以下路徑：

```powershell
.\build\Release\dimenguard.exe demo
```

## 5. 如何證明錯誤公式會被編譯器拒絕？

首先，編譯同一個測試檔案中的正確版本：

```sh
c++ -std=c++20 -I include tests/compile_fail/incompatible_add.cpp -o build-direct/valid_example
```

然後啟用錯誤測試案例：

```sh
c++ -std=c++20 -I include -DDIMENGUARD_NEGATIVE_CASE=1 tests/compile_fail/incompatible_add.cpp -o build-direct/invalid_example
```

第二條指令預期會編譯失敗，因為它嘗試編譯 `1_m + 1_s`。編譯器顯示的錯誤訊息，正是程式成功阻止不相容單位相加的證據。

以上手動指令適用於 GCC／Clang。Windows 使用 MSVC 的使用者可以直接透過 CTest 執行相同測試，無須直接使用以上 compiler flags。

## 6. 建議的原始碼閱讀順序

1. `examples/physics.cpp`：先了解函式庫的基本用法。
2. `include/dimenguard/dimension.hpp`：了解代表三種基本維度的 template 指數。
3. `include/dimenguard/unit.hpp`：了解維度及單位換算比例。
4. `include/dimenguard/quantity.hpp`：了解數值的儲存方式及運算限制。
5. `include/dimenguard/si.hpp`：了解單位別名及 `_m` 等 literals。
6. `include/dimenguard/diagnostics.hpp`：了解維度推導及相關說明功能。

`quantity<metre>` 物件只會儲存一個 `double`。維度和單位屬於編譯器使用的型別資料，並不會以額外字串的形式儲存在每個物件內。輸出格式及解釋文字則由其他功能負責，因此會帶來額外的執行時成本。
