cmake_minimum_required(VERSION 3.16)
project(AlgorithmProject C)

set(CMAKE_C_STANDARD 99)

add_executable(BST bst/bst.c bst/bst.h bst/bst_main.c)
add_executable(MinHeap heap/minheap.c heap/minheap.h heap/minheap_main.c)
add_executable(RBT rbt/rbt.c rbt/rbt.h rbt/rbt_main.c)
add_executable(AVL avl/avl.c avl/avl.h avl/avl_main.c)
add_executable(QuickSelectMain first_part/quickselect_main.c first_part/utils.c
        first_part/utils.h first_part/select_fun.c first_part/select_fun.h)

add_executable(HeapSelectMain first_part/heapselect_main.c first_part/utils.c first_part/utils.h
        first_part/select_fun.c first_part/select_fun.h heap/minheap.c heap/minheap.h
        heap/maxheap.c heap/maxheap.h)

add_executable(MedianSelectMain first_part/medianselect_main.c first_part/select_fun.c
        first_part/select_fun.h first_part/utils.h first_part/utils.c heap/minheap.c
        heap/minheap.h heap/maxheap.c heap/maxheap.h)

add_executable(Times first_part/select_fun.c
        first_part/select_fun.h first_part/utils.h first_part/utils.c heap/minheap.c
        heap/minheap.h heap/maxheap.c heap/maxheap.h first_part/times.c)

target_link_libraries(Times m)