cmake_minimum_required(VERSION 3.21)

file(GLOB_RECURSE SRC src/*.cpp src/*.ixx includes/*.hpp exports/*.hpp)

if (MSVC)
    file(GLOB_RECURSE WIN32_SRC Win32/src/*.cpp Win32/includes/*.hpp)
endif()

add_library(PlutoLib STATIC ${SRC} ${WIN32_SRC})

if(MSVC)
    target_include_directories(PlutoLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/exports/ ${CMAKE_CURRENT_SOURCE_DIR}/includes/ ${CMAKE_CURRENT_SOURCE_DIR}/Win32/includes/)
else()
    target_include_directories(PlutoLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/exports/ ${CMAKE_CURRENT_SOURCE_DIR}/includes/)
endif()

if(MSVC)
    target_compile_definitions(PlutoLib PUBLIC PLUTO_WIN32)
    target_link_libraries(PlutoLib PRIVATE
       Xinput.lib
    )
    target_compile_options(PlutoLib PRIVATE /fp:fast /MP /Ot /W4 /Gy)
endif()
