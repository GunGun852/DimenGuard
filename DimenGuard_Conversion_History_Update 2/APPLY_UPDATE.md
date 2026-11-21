# DimenGuard: make menu option 5 appear

This is a source-file update, not a complete standalone project. It replaces `app/main.cpp` in your original full DimenGuard repository.

## 1. Replace the source file

Copy `app/main.cpp` from this package into the root of your original DimenGuard repository, replacing the existing `app/main.cpp`.

The updated menu source includes:

```cpp
<< "4. Explain dimensions\\n"
<< "5. Show conversion history\\n"
<< "0. Exit\\n\\n";
```

## 2. Rebuild from the original project root (macOS Terminal)

Run these commands in the folder that contains the original project's `CMakeLists.txt`:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel 2
./build/dimenguard
```

Do not launch an older copy of the executable. The new menu appears only after rebuilding and running the newly built executable.

## 3. Expected menu

```text
1. Convert units
2. List supported units
3. Show physics demo
4. Explain dimensions
5. Show conversion history
0. Exit
```

If CMake says it cannot find `CMakeLists.txt`, you are not in the full project root. This package only supplies the changed source file; it does not contain the full project's headers, CMake configuration, or libraries.
