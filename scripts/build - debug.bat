chcp 65001
set BUILD_TYPE=debug
copy vcvars64_vanilla_cmake_config_build.bat "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build"
cmd /k "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64_vanilla_cmake_config_build.bat" %BUILD_TYPE%
