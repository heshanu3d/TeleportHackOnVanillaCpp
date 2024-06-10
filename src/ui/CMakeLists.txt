add_library(ui_dx9 STATIC
        ui_main.cpp
        ${imgui_dir}/backends/imgui_impl_dx9.cpp
)
target_link_libraries(ui_dx9 PRIVATE
        imgui_d3dx_base_obj
        d3d9.lib
        config_parser
)
target_include_directories(ui_dx9 PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)
