cmake_minimum_required(VERSION 3.5)
project(stl C CXX)

include_directories(${CMAKE_SOURCE_DIR}/include)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic -std=gnu99 -fexceptions -Wno-pointer-arith")

file(GLOB STL_SRC ${CMAKE_SOURCE_DIR}/src/*.c)

add_library(stl SHARED ${STL_SRC})
