add_executable(target target.cpp)
add_executable(demo demo.cpp)
target_link_libraries(demo PRIVATE hack)

add_executable(imgui_dx9_demo
        imgui_dx9_demo.cpp
        ${imgui_dir}/backends/imgui_impl_dx9.cpp
)
target_link_libraries(imgui_dx9_demo PRIVATE
        imgui_d3dx_base_obj
        d3d9.lib
)


install(TARGETS target demo imgui_dx9_demo
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION bin
        ARCHIVE DESTINATION lib)