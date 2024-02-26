cmake_minimum_required(VERSION 3.12)
add_library(light_gui)

target_include_directories(
        light_gui PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_sources(
        light_gui PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/src/module.c
        ${CMAKE_CURRENT_LIST_DIR}/src/gui.c
        ${CMAKE_CURRENT_LIST_DIR}/src/object.c
)

target_link_libraries(
        light_gui PUBLIC
        rend
        light_display
        light_framework
)

if(LIGHT_SYSTEM STREQUAL PICO_SDK)
        target_link_libraries(
                light_gui PUBLIC
        )
endif()
