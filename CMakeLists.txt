cmake_minimum_required(VERSION 3.20)
project(BinaryHeap C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(BinaryHeap
        priorityQueues.c
        priorityQueues.h)
