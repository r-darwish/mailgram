cmake_minimum_required(VERSION 3.0)

project(Mailgram)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT MSVC)
    set(COMPILE_OPTIONS
      -Wall
      -Wextra
      -Werror
      -Wconversion)
endif()

add_compile_options(${COMPILE_OPTIONS})

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_subdirectory(src)
