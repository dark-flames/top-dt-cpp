cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(ToyDT CXX)

message("Compiler path ${CMAKE_CXX_COMPILER}")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # require at least gcc 4.8
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 13.1)
        message(FATAL_ERROR "G++ version must be at least 13.1, which is ${CMAKE_CXX_COMPILER_VERSION} now.")
    endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    # require at least clang 3.2
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 17.0.0)
        message(FATAL_ERROR "Clang version must be at least 17, which is ${CMAKE_CXX_COMPILER_VERSION} now.")
    endif()
else()
    message(WARNING "You are using an unsupported compiler: ${CMAKE_CXX_COMPILER_ID}! Compilation has only been tested with Clang and GCC.")
endif()

set(module_list)

include_directories(src)

add_subdirectory(src)
add_subdirectory(bin)