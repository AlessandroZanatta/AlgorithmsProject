//
// Created by Alessandro Zanatta on 7/8/20.
//

#ifndef ALGORITHMPROJECT_MINHEAP_H
#define ALGORITHMPROJECT_MINHEAP_H

#define MAX_LENGTH 100

/**
 * a MinHeap struct. The first index of the array (0 index) represents the current HeapSize
 */
struct MinHeap{
    int * array;
};

struct MinHeap * createEmptyMinHeap();
int heapsizeMin(struct MinHeap * minHeap);
void destroyMinHeap(struct MinHeap * minHeap);
struct MinHeap * buildMinHeap(int * array, int num_elements);
void printElementsMin(struct MinHeap * minHeap);
int getMin(const struct MinHeap * minHeap);
int extractMin(struct MinHeap * minHeap);
void insertMin(struct MinHeap * minHeap, int i);
void changeValueMin(struct MinHeap * maxHeap, int i, int key);

#endif //ALGORITHMPROJECT_MINHEAP_H
