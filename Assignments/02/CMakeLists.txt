cmake_minimum_required(VERSION 3.5)
project(02)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories("D:\\opencv\\build\\include\\")
include_directories("D:\\opencv\\build\\include\\opencv\\")

set(SOURCE_FILES
        src/hw3.c
        src/hw3.h
        src/hw3ex.c
        )

add_executable(02 ${SOURCE_FILES})