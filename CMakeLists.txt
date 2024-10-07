cmake_minimum_required(VERSION 3.21)

project(Pluto
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ADD_TEST_PLUTO "If the test should be built" OFF)

add_subdirectory(library)

if(ADD_TEST_PLUTO)
    enable_testing()
    add_subdirectory(test)
endif()

add_library(razer::pluto ALIAS PlutoLib)
