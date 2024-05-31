cmake_minimum_required(VERSION 3.20)
project(TeleportHackOnVanilla)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES src/main.cpp src/hack.cpp)

add_executable(TeleportHackOnVanilla ${SOURCE_FILES})
add_executable(target test/target.cpp)
add_executable(hack test/demo.cpp)


install(TARGETS TeleportHackOnVanilla target hack
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)