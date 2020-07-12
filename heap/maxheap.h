//
// Created by Alessandro Zanatta on 7/8/20.
//

#ifndef ALGORITHMPROJECT_MAXHEAP_H
#define ALGORITHMPROJECT_MAXHEAP_H

#define MAX_LENGTH 100

/**
 * a MaxHeap struct. The first index of the array (0 index) represents the current HeapSize
 */
struct MaxHeap{
    int * array;
};

struct MaxHeap * createEmptyMaxHeap();
int heapsizeMax(struct MaxHeap * maxHeap);
void destroyMaxHeap(struct MaxHeap * maxHeap);
struct MaxHeap * buildMaxHeap(int * array, int num_elements);
void printElementsMax(struct MaxHeap * maxHeap);
int getMax(const struct MaxHeap * maxHeap);
int extractMax(struct MaxHeap * maxHeap);
void insertMax(struct MaxHeap * maxHeap, int i);
void changeValueMax(struct MaxHeap * maxHeap, int i, int key);



// MAXHEAP WITH POSITION

#define KEY 0
#define POS 1

struct MaxHeapPos{
    int array[MAX_LENGTH][2];
};

struct MaxHeapPos * createEmptyMaxHeapPos();
int heapsizeMaxPos(struct MaxHeapPos * maxHeap);
void destroyMaxHeapPos(struct MaxHeapPos * maxHeap);
struct MaxHeapPos * buildMaxHeapPos(const int * array, int num_elements);
void printElementsMaxPos(struct MaxHeapPos * maxHeap);
int getMaxPos(const struct MaxHeapPos * maxHeap);
void extractMaxPos(struct MaxHeapPos * maxHeap, int * res);
void insertMaxPos(struct MaxHeapPos * maxHeap, int i, int pos);
void changeValueMaxPos(struct MaxHeapPos * maxHeap, int i, int key);
int getKeyOnPositionMax(struct MaxHeap * maxHeap, int i);

#endif //ALGORITHMPROJECT_MAXHEAP_H
