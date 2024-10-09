cmake_minimum_required(VERSION 3.21)

file(GLOB_RECURSE SRC src/*.cpp src/*.ixx includes/*.hpp exports/*.hpp)

if (MSVC)
    file(GLOB_RECURSE WIN32_SRC Win32/src/*.cpp Win32/includes/*.hpp)
endif()

if(ADD_TEST_PLUTO)
    set(LIBRARY_TYPE STATIC)
else()
    set(LIBRARY_TYPE SHARED)
endif()

add_library(PlutoLib ${LIBRARY_TYPE}
    ${SRC} ${WIN32_SRC}
)

if(MSVC)
    set(PLUTO_PRIVATE_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/includes/ ${CMAKE_CURRENT_SOURCE_DIR}/Win32/includes/)
else()
    set(PLUTO_PRIVATE_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/includes/)
endif()

set(PLUTO_PRIVATE_INCLUDES ${PLUTO_PRIVATE_INCLUDES} PARENT_SCOPE)

target_include_directories(PlutoLib PRIVATE ${PLUTO_PRIVATE_INCLUDES})

target_include_directories(PlutoLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/exports/)

target_compile_definitions(PlutoLib PRIVATE BUILD_PLUTO)

if(MSVC)
    target_compile_definitions(PlutoLib PRIVATE PLUTO_WIN32)
    target_link_libraries(PlutoLib PRIVATE
       Xinput.lib
    )
    target_compile_options(PlutoLib PRIVATE /fp:fast /MP /Ot /W4 /Gy)
endif()
