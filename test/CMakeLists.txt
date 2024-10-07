cmake_minimum_required(VERSION 3.21)

if(MSVC)
    file(GLOB_RECURSE WIN32SRC Win32/*.cc)
endif()

add_executable(PlutoTest
    ${WIN32SRC}
)

target_include_directories(PlutoTest PRIVATE ${PLUTO_PRIVATE_INCLUDES})

unset(PLUTO_PRIVATE_INCLUDES)

if(MSVC)
    target_compile_definitions(PlutoTest PRIVATE PLUTO_WIN32)
    target_link_libraries(PlutoTest PRIVATE
       Xinput.lib
    )
    target_compile_options(PlutoTest PRIVATE /fp:fast /MP /Ot /W4 /Gy)
endif()

include(FetchContent)

set(GOOGLE_TEST_COMMIT_ID 12a5852e451baabc79c63a86c634912c563d57bc CACHE STRING "Supply the latest commit ID from the GitHub repository.")

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/${GOOGLE_TEST_COMMIT_ID}.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

target_link_libraries(PlutoTest PRIVATE
    GTest::gtest_main PlutoLib
)

include(GoogleTest)

gtest_discover_tests(PlutoTest)
