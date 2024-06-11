@REM put this file in visual studio 202x cmdline tools directories, such as : C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build

@call "%~dp0vcvarsall.bat" x64
echo  -------------cleaning debug---------------
cd ../build/debug
ninja clean
echo  -------------cleaning debug done---------------

echo  -------------cleaning release---------------
cd ../build/release
ninja clean
echo  -------------cleaning release done---------------
