//
// Created by Alessandro Zanatta on 7/11/20.
//

#ifndef ALGORITHMPROJECT_UTILS_H
#define ALGORITHMPROJECT_UTILS_H

#define BUFFER 10000
#define CHAR_BUFFER 50000

void swap_array(int * a, int x, int y);
int partition(int * a, int start, int end, int key_pos);
int * allocate_array();
int read_array(int * a, char * values, int start_from);
void quicksort(int * a, int p, int q);
void quicksortLong(long * a, int p, int q);
void quicksortDouble(double * a, int p, int q);
int partition_with_pivot(int * a, int start, int end, int pivot);
double getMedianResolution();

#endif //ALGORITHMPROJECT_UTILS_H
