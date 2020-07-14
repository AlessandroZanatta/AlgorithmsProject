//
// Created by Alessandro Zanatta on 7/8/20.
//

#include "minheap.h"
#include <stdlib.h>
#include <stdio.h>

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
 * @param minHeap
 * @param i
 * @return the key on the i-th position of the array
 */
int getKeyOnPositionMin(struct MinHeap * minHeap, int i){
    return minHeap->array[i];
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
    minHeap->array[0] = num_elements;

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

// ----------------------
// MINHEAP WITH POSITIONS
// ----------------------

/**
 * Creates an empty MinHeap (MinHeap with zero elements)
 * @return the pointer to the allocated MinHeap struct
 */
struct MinHeapPos * createEmptyMinHeapPos(int length){
    struct MinHeapPos * newMinHeap = (struct MinHeapPos *) malloc(sizeof(struct MinHeapPos));
    newMinHeap->array = (struct Pair *) malloc(sizeof(struct Pair) * (length+1)); // +1 as we need the first cell as heapsize!

    newMinHeap->array[0].key = 0; // only first cell of the first element in the array is updated as the heapsize!
    return newMinHeap;
}

/**
 * @param minHeap
 * @return the heapsize of the given min-heap
 */
int heapsizeMinPos(struct MinHeapPos * minHeap){
    return minHeap->array[0].key;
}

/**
 * De-allocates a minHeap entirely
 * @param minHeap
 */
void destroyMinHeapPos(struct MinHeapPos * minHeap){
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
void swapMinPos(struct MinHeapPos * minHeap, int i, int j){
    int a = minHeap->array[i].key;
    minHeap->array[i].key = minHeap->array[j].key;
    minHeap->array[j].key = a;

    a = minHeap->array[i].pos;
    minHeap->array[i].pos = minHeap->array[j].pos;
    minHeap->array[j].pos = a;
}

/**
 * This procedure is used to preserves the properties of MinHeaps
 * Assumes that both children of minHeap[i] are sub-trees of a MinHeap
 * @param minHeap
 * @param i
 */
void MinHeapifyPos(struct MinHeapPos * minHeap, int i){
    int l = LEFT(i);
    int r = RIGHT(i);
    int m;

    if(l <= heapsizeMinPos(minHeap) && minHeap->array[i].key > minHeap->array[l].key){
        m = l;
    } else {
        m = i;
    }

    if(r <= heapsizeMinPos(minHeap) && minHeap->array[m].key > minHeap->array[r].key){
        m = r;
    }

    if(m != i){
        swapMinPos(minHeap, i, m);
        MinHeapifyPos(minHeap, m);
    }
}

/**
 * Prints the content of the min-heap
 * @param minHeap
 */
void printElementsMinPos(struct MinHeapPos * minHeap){
    for(int i = 1; i <= heapsizeMinPos(minHeap); i++){
        printf("%d ", minHeap->array[i].key);
        // printf("%d\n", minHeap->array[i].pos);
    }
}

/**
 * @param minHeap
 * @return the min value of the minHeap
 */
int getMinPos(const struct MinHeapPos * minHeap){
    return minHeap->array[1].key;
}

/**
 * Extracts the minimum value of the min-heap and restores its properties
 * @param minHeap
 * @return the min value of the min-heap
 */
void extractMinPos(struct MinHeapPos * minHeap, struct Pair * res){
    res->key = minHeap->array[1].key;
    res->pos = minHeap->array[1].pos;
    minHeap->array[1].key = minHeap->array[heapsizeMinPos(minHeap)].key;
    minHeap->array[1].pos = minHeap->array[heapsizeMinPos(minHeap)].pos;
    minHeap->array[0].key--; // decrement heapsize
    MinHeapifyPos(minHeap, 1);
}

/**
 * Inserts in the given MinHeap the key i
 * Assumes there is enough space in the array
 * @param minHeap
 * @param i
 */
void insertMinPos(struct MinHeapPos * minHeap, int i, int pos){
    minHeap->array[0].key++;
    minHeap->array[heapsizeMinPos(minHeap)].key = i;
    minHeap->array[heapsizeMinPos(minHeap)].pos = pos;

    int x = heapsizeMinPos(minHeap);
    while(PARENT(x) >= 1 && minHeap->array[x].key < minHeap->array[PARENT(x)].key){
        swapMinPos(minHeap, x, PARENT(x));
        x = PARENT(x);
    }
}

/**
 * Changes the value of the i-th element in the heap with the given key and restores min-heap properties
 * @param minHeap
 * @param i : 0 < i <= heapsize
 * @param key
 */
void changeValueMinPos(struct MinHeapPos * minHeap, int i, int key){
    if(minHeap->array[i].key < key){

        minHeap->array[i].key = key;
        MinHeapifyPos(minHeap, i);
    } else if(minHeap->array[i].key > key){

        minHeap->array[i].key = key;

        int x = i;
        while(PARENT(x) >= 1 && minHeap->array[x].key < minHeap->array[PARENT(x)].key){
            swapMinPos(minHeap, x, PARENT(x));
            x = PARENT(x);
        }
    }
}