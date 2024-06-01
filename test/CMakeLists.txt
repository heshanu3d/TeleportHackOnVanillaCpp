add_executable(target target.cpp)
add_executable(demo demo.cpp)

target_link_libraries(demo PRIVATE hack)

install(TARGETS target demo
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION bin
        ARCHIVE DESTINATION lib)