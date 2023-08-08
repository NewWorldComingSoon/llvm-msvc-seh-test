# llvm-msvc-seh-test
SEH test for llvm-msvc.

## Requirements

- Visual Studio 2022
- CMake 3.15+
- [llvm-msvc](https://github.com/NewWorldComingSoon/llvm-msvc-build/releases)

## Building with llvm-msvc

```
cmake -Bbuild -TLLVM-MSVC_v143
cmake --build build --config Release
```
```
cmake -Bbuild32 -TLLVM-MSVC_v143 -AWin32
cmake --build build32 --config Release
```
