//
// Created by Alessandro Zanatta on 7/13/20.
//

#include "../mtwister/mtwister.h"
#include "utils.h"
#include "time.h"
#include <stdio.h>
#include "select_fun.h"
#include <stdlib.h>

#define ITERATIONS 1

/**
 * Computes the median time of a random initialization of a vector of given length
 * @param resolution of the used clock
 * @param length of the array
 * @param seed for prng MT
 * @return the median initialization time
 */
long getInitTime(long resolution, int length, unsigned long seed){

    struct timespec start,end;
    long times [ITERATIONS];
    int array [length];
    int counter = 0;
    MTRand prng;

    for(int i = 0; i < ITERATIONS; i++){
        printf("Getting init time: %d", i);
        clock_gettime(CLOCK_MONOTONIC, &start);
        do{
            prng = seedRand(seed);
            for(int j = 0; j < length; j++){
                array[j] = (int) genRandLong(&prng);
            }

            clock_gettime(CLOCK_MONOTONIC, &end);
            counter++;
        } while((double) (end.tv_nsec - start.tv_nsec) <= (double) resolution * ((1 / 0.005) + 1));
        times[i] = (end.tv_nsec - start.tv_nsec) / counter;

    }

    quicksortLong(times, 0, ITERATIONS - 1);
    return times[(int) (ITERATIONS/2)];
}

/**
 * Gets the time of execution for the given selectFunction algorithm
 * @param resolution of the clock
 * @param length of the array
 * @param seed for the MT prng
 * @param timeInit the time used for initialization
 * @param k the element we want to get
 * @param selectFunction the select function we want to use
 * @param time the function writes the mean and std deviation in time[0] and time[1]
 */
void getSelectTime(long resolution, int length, unsigned long seed, long timeInit, int k, int (*selectFunction)(int *, int, int), long * time){

    struct timespec start,end;
    long times [ITERATIONS];

    // check if we are using heapselect. In this case, we want to leave the first cell of the array empty (heapsize!)
    int real_length = (selectFunction == heapselect) ? length+1 : length;

    int array [real_length];

    int counter = 0;
    MTRand prng;

    for(int i = 0; i < ITERATIONS; i++){

        if(selectFunction == quickselect){
            printf("QuickSelect: %d/%d\n", i, ITERATIONS);
        } else if(selectFunction == heapselect){
            printf("HeapSelect: %d/%d\n", i, ITERATIONS);
        } else {
            printf("MedianSelect: %d/%d\n", i, ITERATIONS);
        }

        clock_gettime(CLOCK_MONOTONIC, &start);
        do{
            prng = seedRand(seed+i);
            for(int j = real_length-length; j < real_length; j++){ // this will leave the first cell free
                array[j] = (int) genRandLong(&prng);
            }

            selectFunction(array, length, k); // length as in both cases there are effectively length elements

            clock_gettime(CLOCK_MONOTONIC, &end);
            counter++;
        } while((end.tv_nsec - start.tv_nsec) <= timeInit + resolution * ((1 / 0.005) + 1));
        times[i] = (end.tv_nsec - start.tv_nsec) / counter;

    }

    for(int i = 0; i < ITERATIONS; i++){
        time[0] = times[i];
        time[1] = times[i] * times[i];
    }

    time[0] = time[0] / ITERATIONS; // E(X)
    time[1] = time[1] / ITERATIONS - time[0] * time[0]; // E(X^2) - (E(X))^2 = sigma(X)
}

/**
 * Converts nanoseconds to seconds
 * @param i
 * @return the seconds
 */
double seconds(long i) {
    return (double) i / 1000000000;
}

int main(){
    long resolution = getMedianResolution();

    int array_length = 100;
    long initTime = 0;
    long quickTime [2];
    long heapTime [2];
    long medianTime [2];

    unsigned long long seed = time(NULL); // get seed as the time since Unix Epoch

    //FILE * output = fopen("times.txt", "w");
    //fprintf(output, "N K T1 D1 T2 D2 T3 D3");
    printf("N K T1 D1 T2 D2 T3 D3\n");
    for(int i = 0; i < 40; i++){

        int k = (int) 2 * array_length / 5; // take k as 2/5 of the length of the array, it does not matter (for most algorithms, see below)


        initTime = getInitTime(resolution, array_length, seed);
        getSelectTime(resolution, array_length, seed, initTime, k, quickselect, quickTime);
        getSelectTime(resolution, array_length, seed, initTime, k, heapselect, heapTime);
        // getSelectTime(resolution, array_length, seed, initTime, k, medianselect, medianTime);

        /*fprintf(output, "%d %d %f %f %f %f %f %f",
                array_length, k,
                seconds(quickTime[0]), seconds(quickTime[1]),
                seconds(heapTime[0]), seconds(heapTime[1]),
                seconds(medianTime[0]), seconds(medianTime[1]));*/

        printf("%d %d ", array_length, k);
        printf("%f %f %f %f %f %f\n",
               seconds(quickTime[0]), seconds(quickTime[1]),
               seconds(heapTime[0]), seconds(heapTime[1]),
               seconds(medianTime[0]), seconds(medianTime[1]));

        array_length = (int) (array_length * 1.32); // doing so it reaches a length of about 5'000'000
    }

    //fclose(output);
}
