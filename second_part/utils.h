//
// Created by Alessandro Zanatta on 7/11/20.
//

#ifndef ALGORITHMPROJECT_UTILS_H
#define ALGORITHMPROJECT_UTILS_H

#include <time.h>

void quicksortDouble(double * a, int p, int q);
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