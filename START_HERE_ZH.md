# DimenGuard：由呢度開始

呢個專案係 **C++20 編譯期單位及維度檢查函式庫**。
目標係畀數值帶有單位型別，等編譯器喺程式執行之前發現指定類型嘅錯誤。
例如米加秒會編譯失敗；距離除以時間就會自動產生速度型別。

主體係 library，同時附有可以直接執行嘅示範程式。

## 1. Mac 最快開始方法

解壓縮 ZIP，用 VS Code 開啟 `DimenGuard` 資料夾，再開 Terminal。
先確認 Terminal 位於包含 `README.md` 同 `CMakeLists.txt` 嗰層。

```sh
bash tools/build_direct.sh
./build-direct/dimenguard demo
```

呢個方法只需要 C++20 compiler。假如未裝 Apple 開發工具，可先執行：

```sh
xcode-select --install
```

完成安裝後再執行 build 指令。舊版本 compiler 若唔支援 C++20，需更新開發工具。
呢份程式喺 Linux/GCC 已驗證；Mac 本機運行仍要由你哋實際測試。

正常會見到：

```text
100 m / 10 s         = 10 m/s
(100 m / 10 s) / 5 s = 2 m/s^2
2 kg * acceleration  = 4 N
1 m + cast<m>(1 ft)  = 1.3048 m
72 km/h in m/s       = 20 m/s
```

## 2. 睇維度推導

```sh
./build-direct/dimenguard explain
```

`L` 代表長度，`M` 代表質量，`T` 代表時間。右上角指數寫成 `^`：

| 物理量 | L | M | T | 解釋 |
|---|---:|---:|---:|---|
| 距離 | 1 | 0 | 0 | 長度 |
| 時間 | 0 | 0 | 1 | 時間 |
| 速度 | 1 | 0 | -1 | 長度／時間 |
| 加速度 | 1 | 0 | -2 | 長度／時間² |
| 力 | 1 | 1 | -2 | 質量 × 加速度 |

乘法將指數相加，除法將指數相減。
例如速度除以時間：`(1,0,-1) - (0,0,1) = (1,0,-2)`，結果係加速度。

`explain` 會用型別資料解釋操作，唔會真係將非法加法寫入主程式。
真正嘅編譯拒絕證據喺 `tests/compile_fail` 同自動測試入面。

## 3. 單位轉換

```sh
./build-direct/dimenguard convert 1 ft m
./build-direct/dimenguard convert 500 g kg
./build-direct/dimenguard convert 72 km/h m/s
./build-direct/dimenguard units
```

米同呎都係長度，可以換算。呢個 library 要求先明確轉換，先可以相加：

```cpp
auto length = 1_m + dimenguard::unit_cast<dimenguard::metre>(1_ft);
```

CLI 嘅單位名稱係用戶執行時輸入，所以錯誤名稱喺執行時檢查；
C++ 原始碼嘅單位型別及公式則喺編譯期檢查。兩者要分清楚。

## 4. 用 CMake 跑完整測試

電腦有 CMake 3.20 或以上時：

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel 2
ctest --test-dir build -C Release --output-on-failure --parallel 2
./build/dimenguard demo
```

如果用 CMake，路徑係 `build`；用 compiler-only script 就係 `build-direct`。
VS Code 入面亦有 configure/build/test/demo tasks。

Windows 可喺已設定 C++ 開發環境嘅 Developer PowerShell 執行以上 CMake 指令。
Visual Studio generator 嘅程式通常喺：

```powershell
.\build\Release\dimenguard.exe demo
```

## 5. 點樣證明錯誤公式真係編譯失敗？

先編譯同一份測試檔嘅正確版本：

```sh
c++ -std=c++20 -I include tests/compile_fail/incompatible_add.cpp -o build-direct/valid_example
```

再開啟錯誤分支：

```sh
c++ -std=c++20 -I include -DDIMENGUARD_NEGATIVE_CASE=1 tests/compile_fail/incompatible_add.cpp -o build-direct/invalid_example
```

第二條指令**預期會失敗**，因為佢嘗試將 `1_m + 1_s` 編譯。
呢個錯誤訊息正正係功能成功運作嘅證據。呢個手動指令適用於 GCC／Clang；
Windows MSVC 可以直接用 CTest 跑同一個測試，唔需要照搬 compiler flags。

## 6. 最值得讀嘅原始碼順序

1. `examples/physics.cpp`：先理解使用方式。
2. `include/dimenguard/dimension.hpp`：三個 template 指數。
3. `include/dimenguard/unit.hpp`：維度＋換算比例。
4. `include/dimenguard/quantity.hpp`：一個數值，以及運算限制。
5. `include/dimenguard/si.hpp`：單位別名及 `_m` 等 literals。
6. `include/dimenguard/diagnostics.hpp`：解釋維度推導。

`quantity<metre>` 個物件只儲存一個 `double`。維度同單位係編譯器型別資料，
唔係每個物件額外攜帶嘅字串。輸出格式同解釋文字係另外嘅功能，會有執行時成本。

## 7. 交功課前

- 五人各自理解、驗證及改進負責模組，留下真實 commits。
- 建立 public Git repository，填上真實組員 Git 帳號。
- 依照 [示範稿](docs/DEMO_SCRIPT.md) 練習一分鐘展示。
- [創新比較](docs/RELATED_WORK.md) 清楚承認現有 library 已有單位安全功能。
- [提交表](docs/SUBMISSION.md) 仍有需要你哋填寫嘅連結及分工資料。

AI 已協助產生初版實作及測試；呢份 ZIP 唔代表已經有公開 repo、五人共同開發歷史，
或者老師已確認創新分。實際參與及最終理解應由團隊補足。
