//
// Created by Alessandro Zanatta on 7/11/20.
//

#ifndef ALGORITHMPROJECT_UTILS_H
#define ALGORITHMPROJECT_UTILS_H

#define BUFFER 10000
#define CHAR_BUFFER 50000
#include <time.h>

void swap_array(int * a, int x, int y);
int partition(int * a, int start, int end, int key_pos);
int * allocate_array();
int read_array(int * a, char * values, int start_from);
void quicksort(int * a, int p, int q);
void quicksortLong(long * a, int p, int q);
void quicksortDouble(double * a, int p, int q);
int partition_with_pivot(int * a, int start, int end, int pivot);
double getMedianResolution();
double getDifference(struct timespec start, struct timespec end);

#endif //ALGORITHMPROJECT_UTILS_H

// Mersenne Twister

#ifndef __MTWISTER_H
#define __MTWISTER_H

#define STATE_VECTOR_LENGTH 624
#define STATE_VECTOR_M      397 /* changes to STATE_VECTOR_LENGTH also require changes to this */

typedef struct tagMTRand {
    unsigned long mt[STATE_VECTOR_LENGTH];
    int index;
} MTRand;

MTRand seedRand(unsigned long seed);
unsigned long genRandLong(MTRand* rand);
double genRand(MTRand* rand);

#endif /* #ifndef __MTWISTER_H */