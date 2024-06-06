cmake_minimum_required(VERSION 3.20)
project(TeleportHackOnVanilla)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(imgui_dir ${CMAKE_CURRENT_LIST_DIR}/third/imgui-1.90.7)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(third)

install(DIRECTORY res/fonts DESTINATION bin FILES_MATCHING PATTERN "*.ttf")