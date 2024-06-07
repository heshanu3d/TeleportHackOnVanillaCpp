add_library(teleport_list_parser SHARED teleport_list_parser.cpp)
target_include_directories(teleport_list_parser PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_library(config_parser OBJECT teleport_list_parser.cpp)
target_include_directories(config_parser PUBLIC ${CMAKE_CURRENT_LIST_DIR})
