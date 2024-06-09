# add_library(teleport_list_parser SHARED teleport_list_parser.cpp)
# target_include_directories(teleport_list_parser PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(config_parser STATIC teleport_list_parser.cpp)
target_include_directories(config_parser PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(config_parser PUBLIC utils)
# target_link_libraries(config_parser PUBLIC $<TARGET_OBJECTS:utils>)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(config_parser PRIVATE DEBUG)
endif()

get_target_property(include_dirs config_parser INCLUDE_DIRECTORIES)
message("INCLUDE_DIRECTORIES of config_parser: ${include_dirs}")

# get_target_property(include_dirs_utils utils INCLUDE_DIRECTORIES)
# get_target_property(include_dirs_utils $<TARGET_OBJECTS:utils> INCLUDE_DIRECTORIES)
# message("INCLUDE_DIRECTORIES of utils: ${include_dirs_utils}")

