cmake_minimum_required(VERSION 3.17)
project(project4)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:AVX2 /utf-8")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
endif()

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(ex1)
add_subdirectory(ex2)
