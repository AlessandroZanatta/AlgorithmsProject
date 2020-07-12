//
// Created by Alessandro Zanatta on 7/12/20.
//

#include "maxheap.h"
#include <stdlib.h>
#include <stdio.h>

#define LEFT(i) (2*i)
#define RIGHT(i) (2*i + 1)
#define PARENT(i) ((int) (i/2))

/**
 * Creates an empty MaxHeap (MaxHeap with zero elements)
 * @return the pointer to the allocated MinHeap struct
 */
struct MaxHeap * createEmptyMaxHeap(){
    struct MaxHeap * newMaxHeap = (struct MaxHeap *) malloc(sizeof(struct MaxHeap));

    newMaxHeap->array = (int *) malloc(sizeof(int) * MAX_LENGTH);
    newMaxHeap->array[0] = 0;
    return newMaxHeap;
}

/**
 * @param maxHeap
 * @return the heapsize of the given max-heap
 */
int heapsizeMax(struct MaxHeap * maxHeap){
    return maxHeap->array[0];
}

/**
 * De-allocates a maxheap entirely
 * @param maxHeap
 */
void destroyMaxHeap(struct MaxHeap * maxHeap){
    free(maxHeap->array);
    free(maxHeap);
}

/**
 * Swaps two values in the inner representation of the MinHeap
 * Assumes that both i and j are not zero (or negative) and that they do not exceed the length of the heap size
 * @param minHeap
 * @param i
 * @param j
 */
void swapMax(struct MaxHeap * maxHeap, int i, int j){
    int a = maxHeap->array[i];
    maxHeap->array[i] = maxHeap->array[j];
    maxHeap->array[j] = a;
}

/**
 * This procedure is used to preserves the properties of MaxHeaps
 * Assumes that both children of maxHeap[i] are sub-trees of a MaxHeap
 * @param maxHeap
 * @param i
 */
void MaxHeapify(struct MaxHeap * maxHeap, int i){
    int l = LEFT(i);
    int r = RIGHT(i);
    int m;

    if(l <= heapsizeMax(maxHeap) && maxHeap->array[i] < maxHeap->array[l]){
        m = l;
    } else {
        m = i;
    }

    if(r <= heapsizeMax(maxHeap) && maxHeap->array[m] < maxHeap->array[r]){
        m = r;
    }

    if(m != i){
        swapMax(maxHeap, i, m);
        MaxHeapify(maxHeap, m);
    }
}

/**
 * Assumes the array has the 0-indexed cell NOT used
 * @param array containing elements of the heap
 * @param num_elements of the array (the first index is NOT counted!)
 */
struct MaxHeap * buildMaxHeap(int * array, int num_elements){
    struct MaxHeap * maxHeap = (struct MaxHeap *) malloc(sizeof(struct MaxHeap));

    maxHeap->array = array;
    maxHeap->array[0] = num_elements - 1;

    for(int i = (int) heapsizeMax(maxHeap) / 2; i > 0; i--){
        MaxHeapify(maxHeap, i);
    }

    return maxHeap;
}

/**
 * Prints the content of the max-heap
 * @param maxHeap
 */
void printElementsMax(struct MaxHeap * maxHeap){
    for(int i = 1; i <= heapsizeMax(maxHeap); i++){
        printf("%d ", maxHeap->array[i]);
    }
}

/**
 * @param maxHeap
 * @return the max value of the maxHeap
 */
int getMax(const struct MaxHeap * maxHeap){
    return maxHeap->array[1];
}

/**
 * Extracts the minimum value of the max-heap and restores its properties
 * @param maxHeap
 * @return the max value of the max-heap
 */
int extractMax(struct MaxHeap * maxHeap){
    int max = maxHeap->array[1];
    maxHeap->array[1] = maxHeap->array[heapsizeMax(maxHeap)];
    maxHeap->array[0]--; // decrement heapsize
    MaxHeapify(maxHeap, 1);

    return max;
}

/**
 * Inserts in the given MaxHeap the key i
 * Assumes there is enough space in the array
 * @param maxHeap
 * @param i
 */
void insertMax(struct MaxHeap * maxHeap, int i){
    maxHeap->array[0]++;
    maxHeap->array[maxHeap->array[0]] = i;

    int x = maxHeap->array[0];
    while(PARENT(x) >= 1 && maxHeap->array[x] > maxHeap->array[PARENT(x)]){
        swapMax(maxHeap, x, PARENT(x));
        x = PARENT(x);
    }
}

/**
 * Changes the value of the i-th element in the heap with the given key and restores max-heap properties
 * @param maxHeap
 * @param i : 0 < i <= heapsize
 * @param key
 */
void changeValueMax(struct MaxHeap * maxHeap, int i, int key){
    if(maxHeap->array[i] < key){

        maxHeap->array[i] = key;
        MaxHeapify(maxHeap, i);
    } else if(maxHeap->array[i] > key){

        maxHeap->array[i] = key;

        int x = i;
        while(PARENT(x) >= 1 && maxHeap->array[x] > maxHeap->array[PARENT(x)]){
            swapMax(maxHeap, x, PARENT(x));
            x = PARENT(x);
        }
    }
}