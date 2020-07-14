//
// Created by Alessandro Zanatta on 7/8/20.
//

#ifndef ALGORITHMPROJECT_MINHEAP_H
#define ALGORITHMPROJECT_MINHEAP_H

#define MAX_LENGTH 2500000

#define LEFT(i) (2*i)
#define RIGHT(i) (2*i + 1)
#define PARENT(i) ((int) (i/2))

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
int getKeyOnPositionMin(struct MinHeap * minHeap, int i);

// MINHEAP WITH POSITION

#ifndef HEAP_PAIR
#define HEAP_PAIR
struct Pair{
    int key;
    int pos;
};
#endif


struct MinHeapPos{
    struct Pair * array;
};

struct MinHeapPos * createEmptyMinHeapPos();
int heapsizeMinPos(struct MinHeapPos *maxHeap);
void destroyMinHeapPos(struct MinHeapPos * minHeap);
void printElementsMinPos(struct MinHeapPos * minHeap);
int getMinPos(const struct MinHeapPos * minHeap);
void extractMinPos(struct MinHeapPos * minHeap, struct Pair * res);
void insertMinPos(struct MinHeapPos * minHeap, int i, int pos);
void changeValueMinPos(struct MinHeapPos * maxHeap, int i, int key);

#endif //ALGORITHMPROJECT_MINHEAP_H