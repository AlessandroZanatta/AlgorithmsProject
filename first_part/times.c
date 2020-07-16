//
// Created by Alessandro Zanatta on 7/13/20.
//

#include "utils.h"
#include "time.h"
#include <stdio.h>
#include "select_fun.h"
#include <stdlib.h>
#include <math.h>

#define ITERATIONS 60

/**
 * Computes the median time of a random initialization of a vector of given length
 * @param resolution of the used clock
 * @param length of the array
 * @param seed for prng MT
 * @return the median initialization time
 */
double getInitTime(double resolution, int length, unsigned long seed){

    struct timespec start,end;
    double times [ITERATIONS];
    int * array = (int *) malloc(sizeof(int) * length);
    int counter = 0;
    MTRand prng;

    for(int i = 0; i < ITERATIONS; i++){

        clock_gettime(CLOCK_MONOTONIC_RAW, &start);
        do{
            prng = seedRand(seed+i);
            for(int j = 0; j < length; j++){
                array[j] = (int) genRandLong(&prng);
            }

            clock_gettime(CLOCK_MONOTONIC_RAW, &end);
            counter++;
        } while(getDifference(start, end) <= resolution * ((1 / 0.005) + 1));
        times[i] = getDifference(start, end) / counter;

    }

    quicksortDouble(times, 0, ITERATIONS - 1);
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
void getSelectTime(double resolution, int length, unsigned long seed, double timeInit, int k, int (*selectFunction)(int *, int, int), double * time){

    struct timespec start,end;
    double times [ITERATIONS];

    // check if we are using heapselect. In this case, we want to leave the first cell of the array empty (heapsize!)
    int real_length = (selectFunction == heapselect) ? length+1 : length;

    int * array = (int *) malloc(sizeof(int) * real_length);

    int counter = 0;
    MTRand prng;

    for(int i = 0; i < ITERATIONS; i++){

        clock_gettime(CLOCK_MONOTONIC_RAW, &start);
        do{
            prng = seedRand(seed+i);
            for(int j = real_length-length; j < real_length; j++){ // this will leave the first cell free
                array[j] = (int) genRandLong(&prng);
            }

            selectFunction(array, length, k); // length as in both cases there are effectively length elements

            clock_gettime(CLOCK_MONOTONIC_RAW, &end);
            counter++;
        } while(getDifference(start, end) <= timeInit + resolution * ((1 / 0.005) + 1));
        times[i] = getDifference(start, end) / counter - timeInit; // - timeInit as i don't have to count initialization time

    }

    free(array);

    /*
    double mean = 0;
    double std = 0;

    for(int i = 0; i < ITERATIONS; i++){
        mean += times[i];
        std += (times[i] * times[i]);
    }

    mean /= ITERATIONS;
    std = sqrt(std/ITERATIONS - mean*mean);

    time[0] = mean;
    time[1] = std;
    */

    // Using medians
    quicksortDouble(times, 0, ITERATIONS-1);

    time[0] = times[(int) (ITERATIONS / 2)];
    for(int i = 0; i < ITERATIONS; i++){
        times[i] = (time[0] - times[i]) >= 0 ? (time[0] - times[i]) : (time[0] - times[i])*(-1); // abs(time[0] - times[i])
    }

    quicksortDouble(times, 0, ITERATIONS-1);

    time[1] = times[(int) (ITERATIONS / 2)];

}


int main(){
    double resolution = getMedianResolution();

    int array_length = 100;
    double initTime = 0;
    double quickTime [2];
    double heapTime [2];
    double medianTime [2];
    FILE * output;

    unsigned long long seed = time(NULL); // get seed as the time since Unix Epoch

    printf("Resolution %.17g\n", resolution);

    output = fopen("../first_part/times/basic_times.txt", "w");
    fprintf(output, "N,K,T1,D1,T2,D2,T3,D3\n");
    printf("N K T1 D1 T2 D2 T3 D3\n");
    for(int i = 0; i < 40; i++){

        int k = (int) array_length/3;

        printf("%d %d ", array_length, k);

        initTime = getInitTime(resolution, array_length, seed);

        getSelectTime(resolution, array_length, seed, initTime, k, quickselect, quickTime);
        printf("%.17g %.17g  ", quickTime[0], quickTime[1]);

        getSelectTime(resolution, array_length, seed, initTime, k, heapselect, heapTime);
        printf("%.17g %.17g ", heapTime[0], heapTime[1]);

        getSelectTime(resolution, array_length, seed, initTime, k, medianselect, medianTime);
        printf("%.17g %.17g\n", medianTime[0], medianTime[1]);

        fprintf(output, "%d,%d,%.17g,%.17g,%.17g,%.17g,%.17g,%.17g\n",
                array_length, k,
                quickTime[0], quickTime[1],
                heapTime[0], heapTime[1],
                medianTime[0], medianTime[1]);

        array_length = (int) (array_length * 1.32); // doing so it reaches a length of about 5'000'000
    }

    fclose(output);


    // Evaluate algorithms when k changes using a sample of 75.000 elements
    // We should only see a difference using heap-select
    output = fopen("../first_part/times/changing_k.txt", "w");
    fprintf(output, "N,K,T1,D1,T2,D2,T3,D3\n");
    printf("N K T1 D1 T2 D2 T3 D3\n");
    array_length = 75000;
    int num_points = 250;
    for(int k = 0; k < array_length; k += array_length/num_points){

        initTime = getInitTime(resolution, array_length, seed);
        getSelectTime(resolution, array_length, seed, initTime, k, heapselect, heapTime);
        getSelectTime(resolution, array_length, seed, initTime, k, quickselect, quickTime);
        getSelectTime(resolution, array_length, seed, initTime, k, medianselect, medianTime);
        printf("%d %d %.17g %.17g %.17g %.17g %.17g %.17g\n",
                array_length, k,
                quickTime[0], quickTime[1],
                heapTime[0], heapTime[1],
                medianTime[0], medianTime[1]);

        fprintf(output, "%d,%d,%.17g,%.17g,%.17g,%.17g,%.17g,%.17g\n",
                array_length, k,
                quickTime[0], quickTime[1],
                heapTime[0], heapTime[1],
                medianTime[0], medianTime[1]);
    }

    fclose(output);

    // Evalute heap-select when k and n changes
    output = fopen("../first_part/times/heap3D.txt", "w");
    fprintf(output, "N,K,T2,D2\n");
    array_length = 100;
    num_points = 10;
    for(int i = 0; i < 30; i++){
        for(int k = 0; k < array_length; k += (int) array_length/num_points){

            initTime = getInitTime(resolution, array_length, seed);
            getSelectTime(resolution, array_length, seed, initTime, k, heapselect, heapTime);
            printf("%d,%d,%.17g,%.17g\n", array_length, k, heapTime[0], heapTime[1]);

            fprintf(output, "%d,%d,%.17g,%.17g\n", array_length, k, heapTime[0], heapTime[1]);
        }
        array_length = (int) (array_length * 1.32);
    }

    fclose(output);

    // Evaluate algorithms difference for k = 1, as heap-select should be pretty fast (Theta(n), in theory)
    output = fopen("../first_part/times/times_k1.txt", "w");
    fprintf(output, "N,K,T1,D1,T2,D2,T3,D3\n");
    printf("N K T1 D1 T2 D2 T3 D3\n");

    array_length = 100;
    int k = 1;
    for(int i = 0; i < 40; i++){

        printf("%d %d ", array_length, k);

        initTime = getInitTime(resolution, array_length, seed);

        getSelectTime(resolution, array_length, seed, initTime, k, quickselect, quickTime);
        printf("%.17g %.17g  ", quickTime[0], quickTime[1]);

        getSelectTime(resolution, array_length, seed, initTime, k, heapselect, heapTime);
        printf("%.17g %.17g ", heapTime[0], heapTime[1]);

        getSelectTime(resolution, array_length, seed, initTime, k, medianselect, medianTime);
        printf("%.17g %.17g\n", medianTime[0], medianTime[1]);

        fprintf(output, "%d,%d,%.17g,%.17g,%.17g,%.17g,%.17g,%.17g\n",
                array_length, k,
                quickTime[0], quickTime[1],
                heapTime[0], heapTime[1],
                medianTime[0], medianTime[1]);

        array_length = (int) (array_length * 1.32);
    }

    fclose(output);

}
