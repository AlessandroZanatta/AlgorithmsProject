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
    maxHeap->array[0] = num_elements;

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

/**
 * @param maxHeap
 * @param i
 * @return the key on the i-th position of the array
 */
int getKeyOnPositionMax(struct MaxHeap * maxHeap, int i){
    return maxHeap->array[i];
}

// ----------------------
// MAXHEAP WITH POSITIONS
// ----------------------

/**
 * Creates an empty MaxHeapPos
 * @return the pointer to the allocated MaxHeapPos struct
 */
struct MaxHeapPos * createEmptyMaxHeapPos(){
    struct MaxHeapPos * newMaxHeap = (struct MaxHeapPos *) malloc(sizeof(struct MaxHeapPos));

    newMaxHeap->array[0][0] = 0; // only first cell of the first element in the array is updated as the heapsize!
    return newMaxHeap;
}

/**
 * @param maxHeap
 * @return the heapsize of the given max-heap
 */
int heapsizeMaxPos(struct MaxHeapPos * maxHeap){
    return maxHeap->array[0][0];
}

/**
 * De-allocates a maxHeap entirely
 * @param maxHeap
 */
void destroyMaxHeapPos(struct MaxHeapPos * maxHeap){
    free(maxHeap);
}

/**
 * Swaps two values in the inner representation of the MaxHeapPos
 * Assumes that both i and j are not zero (or negative) and that they do not exceed the length of the heap size
 * @param maxHeap
 * @param i
 * @param j
 */
void swapMaxPos(struct MaxHeapPos * maxHeap, int i, int j){
    int a = maxHeap->array[i][KEY];
    maxHeap->array[i][KEY] = maxHeap->array[j][KEY];
    maxHeap->array[j][KEY] = a;

    a = maxHeap->array[i][POS];
    maxHeap->array[i][POS] = maxHeap->array[j][POS];
    maxHeap->array[j][POS] = a;
}

/**
 * This procedure is used to preserves the properties of MaxHeaps
 * Assumes that both children of maxHeap[i] are sub-trees of a MaxHeap
 * @param maxHeap
 * @param i
 */
void MaxHeapifyPos(struct MaxHeapPos * maxHeap, int i){
    int l = LEFT(i);
    int r = RIGHT(i);
    int m;

    if(l <= heapsizeMaxPos(maxHeap) && maxHeap->array[i][KEY] < maxHeap->array[l][KEY]){
        m = l;
    } else {
        m = i;
    }

    if(r <= heapsizeMaxPos(maxHeap) && maxHeap->array[m][KEY] < maxHeap->array[r][KEY]){
        m = r;
    }

    if(m != i){
        swapMaxPos(maxHeap, i, m);
        MaxHeapifyPos(maxHeap, m);
    }
}

/**
 * Assumes the array has the 0-indexed cell NOT used
 * @param array containing elements of the heap
 * @param num_elements of the array (the first index is NOT counted!)
 */
struct MaxHeapPos * buildMaxHeapPos(const int * array, int num_elements){
    struct MaxHeapPos * maxHeap = (struct MaxHeapPos *) malloc(sizeof(struct MaxHeapPos));

    for(int i = 1; i <= num_elements; i++){
        maxHeap->array[i][KEY] = array[i];
        maxHeap->array[i][POS] = i;
    }
    maxHeap->array[0][0] = num_elements;

    for(int i = (int) heapsizeMaxPos(maxHeap) / 2; i > 0; i--){
        MaxHeapifyPos(maxHeap, i);
    }

    return maxHeap;
}

/**
 * Prints the content of the max-heap
 * @param maxHeap
 */
void printElementsMaxPos(struct MaxHeapPos * maxHeap){
    for(int i = 1; i <= heapsizeMaxPos(maxHeap); i++){
        printf("%d ", maxHeap->array[i][KEY]);
        // printf("%d\n", minHeap->array[i][POS]);
    }
}

/**
 * @param minHeap
 * @return the min value of the minHeap
 */
int getMaxPos(const struct MaxHeapPos * maxHeap){
    return maxHeap->array[1][KEY];
}

/**
 * Extracts the max value of the max-heap and restores its properties
 * @param maxHeap
 * @return the min value of the max-heap
 */
void extractMaxPos(struct MaxHeapPos * maxHeap, int * res){
    res[0] = maxHeap->array[1][KEY];
    res[1] = maxHeap->array[1][POS];
    maxHeap->array[1][KEY] = maxHeap->array[heapsizeMaxPos(maxHeap)][KEY];
    maxHeap->array[1][POS] = maxHeap->array[heapsizeMaxPos(maxHeap)][POS];
    maxHeap->array[0][0]--; // decrement heapsize
    MaxHeapifyPos(maxHeap, 1);
}

/**
 * Inserts in the given MaxHeap the key i
 * Assumes there is enough space in the array
 * @param maxHeap
 * @param i
 */
void insertMaxPos(struct MaxHeapPos * maxHeap, int i, int pos){
    maxHeap->array[0][0]++;
    maxHeap->array[heapsizeMaxPos(maxHeap)][KEY] = i;
    maxHeap->array[heapsizeMaxPos(maxHeap)][POS] = pos;

    int x = heapsizeMaxPos(maxHeap);
    while(PARENT(x) >= 1 && maxHeap->array[x][KEY] > maxHeap->array[PARENT(x)][KEY]){
        swapMaxPos(maxHeap, x, PARENT(x));
        x = PARENT(x);
    }
}

/**
 * Changes the value of the i-th element in the heap with the given key and restores max-heap properties
 * @param maxHeap
 * @param i : 0 < i <= heapsize
 * @param key
 */
void changeValueMaxPos(struct MaxHeapPos * maxHeap, int i, int key){
    if(maxHeap->array[i][KEY] < key){

        maxHeap->array[i][KEY] = key;
        MaxHeapifyPos(maxHeap, i);
    } else if(maxHeap->array[i][KEY] > key){

        maxHeap->array[i][KEY] = key;

        int x = i;
        while(PARENT(x) >= 1 && maxHeap->array[x][KEY] > maxHeap->array[PARENT(x)][KEY]){
            swapMaxPos(maxHeap, x, PARENT(x));
            x = PARENT(x);
        }
    }
}