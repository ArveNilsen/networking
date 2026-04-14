cmake_minimum_required(VERSION 4.2.1)

enable_testing()

if (NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 23)
endif()

set(CMAKE_CXX_EXTENSIONS OFF)

project(networking
    LANGUAGES CXX
)

add_library(project_warnings INTERFACE)
target_compile_options(project_warnings
    INTERFACE
        -Wall
        -Wextra
        -Wpedantic
)

add_library(project_options INTERFACE)
target_compile_features(project_options 
    INTERFACE 
    cxx_std_${CMAKE_CXX_STANDARD}
)

add_subdirectory(src)
add_subdirectory(test)
