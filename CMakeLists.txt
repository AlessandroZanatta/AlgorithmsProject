cmake_minimum_required(VERSION 3.16)
project(AlgorithmProject C)

set(CMAKE_C_STANDARD 99)

add_executable(Bst bst/bst.c bst/bst.h bst/bst_main.c)
add_executable(MinHeap heap/minheap.c heap/minheap.h heap/minheap_main.c)