//
// Created by Alessandro Zanatta on 7/11/20.
//

#ifndef ALGORITHMPROJECT_UTILS_H
#define ALGORITHMPROJECT_UTILS_H

#define NUM_LENGTH 3
#define BUFFER 10000
#define CHAR_BUFFER 50000

void swap_array(int * a, int x, int y);
int partition(int * a, int start, int end, int key_pos);
int * allocate_array();
int read_array(int * a, char * values);

#endif //ALGORITHMPROJECT_UTILS_H
