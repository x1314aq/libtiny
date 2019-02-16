cmake_minimum_required(VERSION 3.0)
project(libtiny C)

include_directories(${CMAKE_SOURCE_DIR}/include)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m64 -Wall -Wextra -Werror -std=gnu99 -fstack-protector-all")

if(CMAKE_C_COMPILER_ID STREQUAL "Clang")
    message(STATUS  "Compiler: Clang")
    set(clang ON CACHE BOOL "use Clang compiler")
elseif (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    message(STATUS  "Compiler: GCC")
    set(gcc ON CACHE BOOL "use GCC compiler")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-implicit-fallthrough")
endif()

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE  "Release")
endif()

# Check platform
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    cmake_policy(SET  CMP0042 NEW)
endif()

file(GLOB SRC ${CMAKE_SOURCE_DIR}/src/*.c)

add_library(tiny SHARED ${SRC})

enable_testing()
add_subdirectory(test)
