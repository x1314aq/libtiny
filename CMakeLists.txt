cmake_minimum_required(VERSION 3.5)
project(stl C CXX)

include_directories(${CMAKE_SOURCE_DIR}/include)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic -std=gnu99 -fexceptions -Wno-pointer-arith")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

file(GLOB header ${CMAKE_SOURCE_DIR}/include/*.h)
file(GLOB src ${CMAKE_SOURCE_DIR}/src/*.c)

add_library(stl STATIC ${src} ${header})
add_executable(qwe main.c)

target_link_libraries(qwe stl)