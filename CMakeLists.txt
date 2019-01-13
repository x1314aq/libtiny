cmake_minimum_required(VERSION 2.8)
project(libtiny C)

include_directories(${CMAKE_SOURCE_DIR}/include)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic -std=gnu99 -fexceptions -Wno-pointer-arith -Wno-newline-eof")

file(GLOB SRC ${CMAKE_SOURCE_DIR}/src/*.c)

add_library(tiny SHARED ${SRC})
