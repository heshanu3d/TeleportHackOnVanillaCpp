# build
## 1
set visual cmdline env first
```
open a cmd and input this to set env
"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
```
then generate and configure with cmake
```
cmake --preset debug
cmake --build build/debug --target all
```

## 2
set env first time build or build directory is deleted
double click scripts/env.bat
double click scripts/build.bat

# clean
## 1
```
cmake --build build/debug --target clean
```

## 2
double click scripts/clean.bat

# install
```
cmake --build build/debug --target install
```

# CMakePresets.json
## how to get
```
Press ctrl+s in CMakeLists.txt at Visual Studio 202x
Infomations will show at output view like below
the cmake generate sentences are in it. 
just copy it out into CmakePresets.json
...
1> 命令行: "C:\Windows\system32\cmd.exe" /c "%SYSTEMROOT%\System32\chcp.com 65001 >NUL && "C:\PROGRAM FILES\MICROSOFT VISUAL STUDIO\2022\COMMUNITY\COMMON7\IDE\COMMONEXTENSIONS\MICROSOFT\CMAKE\CMake\bin\cmake.exe"  -G "Ninja"  -DCMAKE_BUILD_TYPE:STRING="Debug" -DCMAKE_INSTALL_PREFIX:STRING="D:/code/c++/TeleportHackOnVanilla/install/Debug" -DCMAKE_C_COMPILER:STRING="C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.34.31933/bin/Hostx64/x64/cl.exe" -DCMAKE_CXX_COMPILER:STRING="C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.34.31933/bin/Hostx64/x64/cl.exe" -DCMAKE_MAKE_PROGRAM:STRING="C:/Program Files/Microsoft Visual Studio/2022/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/Ninja/ninja.exe"  "D:\code\c++\TeleportHackOnVanilla" 2>&1"
1> 工作目录: D:/code/c++/TeleportHackOnVanilla/build/Debug
1> [CMake] -- Configuring done
1> [CMake] -- Generating done
1> [CMake] -- Build files have been written to: D:/code/c++/TeleportHackOnVanilla/build/Debug
...

```