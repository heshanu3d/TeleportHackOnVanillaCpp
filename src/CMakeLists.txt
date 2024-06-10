add_subdirectory(config_parser)
add_subdirectory(hack)
add_subdirectory(ui)
add_subdirectory(utils)

add_executable(TeleportHackOnVanilla main.cpp)

target_link_libraries(TeleportHackOnVanilla PRIVATE hack ui_dx9)

install(TARGETS TeleportHackOnVanilla
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION bin
        ARCHIVE DESTINATION lib
        )

install(FILES $<TARGET_RUNTIME_DLLS:TeleportHackOnVanilla> TYPE BIN)