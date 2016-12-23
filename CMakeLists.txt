cmake_minimum_required(VERSION 3.6)
project(hw3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.c list.c list.h graph.h set.c set.h graph.c)
#set(SOURCE_FILES test.c list.c list.h set.c set.h graph.c graph.h)
add_executable(hw3 ${SOURCE_FILES})