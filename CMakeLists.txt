cmake_minimum_required(VERSION 3.0)
project(libtiny C)

include_directories(${CMAKE_SOURCE_DIR}/include)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m64 -Wall -Wextra -Werror -std=gnu99")

if(CMAKE_C_COMPILER_ID STREQUAL "Clang")
    message(STATUS  "Compiler: Clang")
elseif (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    message(STATUS  "Compiler: GCC")
endif()

# Check platform
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    cmake_policy(SET  CMP0042 NEW)
endif()

file(GLOB SRC ${CMAKE_SOURCE_DIR}/src/*.c)

add_library(tiny SHARED ${SRC})
