//
// Created by Alessandro Zanatta on 7/8/20.
//

#include "minheap.h"
#include <stdlib.h>
#include <stdio.h>

#define LEFT(i) (2*i)
#define RIGHT(i) (2*i + 1)
#define PARENT(i) ((int) (i/2))

/**
 * Creates an empty MinHeap (MinHeap with zero elements)
 * @return the pointer to the allocated MinHeap struct
 */
struct MinHeap * createEmptyMinHeap(){
    struct MinHeap * newMinHeap = (struct MinHeap *) malloc(sizeof(struct MinHeap));

    newMinHeap->array = (int *) malloc(sizeof(int) * MAX_LENGTH);
    newMinHeap->array[0] = 0;
    return newMinHeap;
}

/**
 * @param minHeap
 * @return the heapsize of the given min-heap
 */
int heapsizeMin(struct MinHeap * minHeap){
    return minHeap->array[0];
}

/**
 * De-allocates a minHeap entirely
 * @param minHeap
 */
void destroyMinHeap(struct MinHeap * minHeap){
    free(minHeap->array);
    free(minHeap);
}

/**
 * Swaps two values in the inner representation of the MinHeap
 * Assumes that both i and j are not zero (or negative) and that they do not exceed the length of the heap size
 * @param minHeap
 * @param i
 * @param j
 */
void swapMin(struct MinHeap * minHeap, int i, int j){
    int a = minHeap->array[i];
    minHeap->array[i] = minHeap->array[j];
    minHeap->array[j] = a;
}

/**
 * This procedure is used to preserves the properties of MinHeaps
 * Assumes that both children of minHeap[i] are sub-trees of a MinHeap
 * @param minHeap
 * @param i
 */
void MinHeapify(struct MinHeap * minHeap, int i){
    int l = LEFT(i);
    int r = RIGHT(i);
    int m;

    if(l <= heapsizeMin(minHeap) && minHeap->array[i] > minHeap->array[l]){
        m = l;
    } else {
        m = i;
    }

    if(r <= heapsizeMin(minHeap) && minHeap->array[m] > minHeap->array[r]){
        m = r;
    }

    if(m != i){
        swapMin(minHeap, i, m);
        MinHeapify(minHeap, m);
    }
}

/**
 * Assumes the array has the 0-indexed cell NOT used
 * @param array containing elements of the heap
 * @param num_elements of the array (the first index is NOT counted!)
 */
struct MinHeap * buildMinHeap(int * array, int num_elements){
    struct MinHeap * minHeap = (struct MinHeap *) malloc(sizeof(struct MinHeap));

    minHeap->array = array;
    minHeap->array[0] = num_elements - 1;

    for(int i = (int) heapsizeMin(minHeap) / 2; i > 0; i--){
        MinHeapify(minHeap, i);
    }

    return minHeap;
}

/**
 * Prints the content of the min-heap
 * @param minHeap
 */
void printElementsMin(struct MinHeap * minHeap){
    for(int i = 1; i <= heapsizeMin(minHeap); i++){
        printf("%d ", minHeap->array[i]);
    }
}

/**
 * @param minHeap
 * @return the min value of the minHeap
 */
int getMin(const struct MinHeap * minHeap){
    return minHeap->array[1];
}

/**
 * Extracts the minimum value of the min-heap and restores its properties
 * @param minHeap
 * @return the min value of the min-heap
 */
int extractMin(struct MinHeap * minHeap){
    int min = minHeap->array[1];
    minHeap->array[1] = minHeap->array[minHeap->array[0]];
    minHeap->array[0]--; // decrement heapsize
    MinHeapify(minHeap, 1);

    return min;
}

/**
 * Inserts in the given MinHeap the key i
 * Assumes there is enough space in the array
 * @param minHeap
 * @param i
 */
void insertMin(struct MinHeap * minHeap, int i){
    minHeap->array[0]++;
    minHeap->array[minHeap->array[0]] = i;

    int x = minHeap->array[0];
    while(PARENT(x) >= 1 && minHeap->array[x] < minHeap->array[PARENT(x)]){
        swapMin(minHeap, x, PARENT(x));
        x = PARENT(x);
    }
}

/**
 * Changes the value of the i-th element in the heap with the given key and restores min-heap properties
 * @param minHeap
 * @param i : 0 < i <= heapsize
 * @param key
 */
void changeValueMin(struct MinHeap * minHeap, int i, int key){
    if(minHeap->array[i] < key){

        minHeap->array[i] = key;
        MinHeapify(minHeap, i);
    } else if(minHeap->array[i] > key){

        minHeap->array[i] = key;

        int x = i;
        while(PARENT(x) >= 1 && minHeap->array[x] < minHeap->array[PARENT(x)]){
            swapMin(minHeap, x, PARENT(x));
            x = PARENT(x);
        }
    }
}