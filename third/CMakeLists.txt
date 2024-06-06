# useless temperally
# set(d3dx_x86_dir "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.19041.0/um/x86")
# set(d3dx_x64_dir "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.19041.0/um/x64")

file(GLOB imgui_d3dx_base_src ${imgui_dir}/imgui*.cpp)
list(APPEND imgui_d3dx_base_src ${imgui_dir}/backends/imgui_impl_win32.cpp)

add_library(imgui_d3dx_base_obj OBJECT ${imgui_d3dx_base_src})
target_include_directories(imgui_d3dx_base_obj PUBLIC
        ${imgui_dir}
        ${imgui_dir}/backends
)

add_executable(imgui_demo_dx9
        ${imgui_dir}/examples/example_win32_directx9/main.cpp
        ${imgui_dir}/backends/imgui_impl_dx9.cpp
)
target_link_libraries(imgui_demo_dx9 PRIVATE
        imgui_d3dx_base_obj
        d3d9.lib
)

add_executable(imgui_demo_dx10
        ${imgui_dir}/examples/example_win32_directx10/main.cpp
        ${imgui_dir}/backends/imgui_impl_dx10.cpp
)
target_link_libraries(imgui_demo_dx10 PRIVATE
        imgui_d3dx_base_obj
        d3d10.lib d3dcompiler.lib
)

add_executable(imgui_demo_dx11
        ${imgui_dir}/examples/example_win32_directx11/main.cpp
        ${imgui_dir}/backends/imgui_impl_dx11.cpp
)
target_link_libraries(imgui_demo_dx11 PRIVATE
        imgui_d3dx_base_obj
        d3d11.lib d3dcompiler.lib
)

add_executable(imgui_demo_dx12
        ${imgui_dir}/examples/example_win32_directx12/main.cpp
        ${imgui_dir}/backends/imgui_impl_dx12.cpp
)
target_link_libraries(imgui_demo_dx12 PRIVATE
        imgui_d3dx_base_obj
        d3d12.lib d3dcompiler.lib dxgi.lib
)

install(TARGETS imgui_demo_dx9 imgui_demo_dx10 imgui_demo_dx11 imgui_demo_dx12
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION bin
        ARCHIVE DESTINATION lib)