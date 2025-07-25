#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct VertsArrayInfo {//TODO: This func prolly shouldnt be in this file. 
    size_t size_buff;
    int max_value;
    int* array;
    float** vertsArr;
    size_t totalVertArr;
}VertsArrayInfo;

VertsArrayInfo get_vertArrayInfo();//TODO: This func prolly shouldnt be in this file. 

float* generateArrayBarVertices(VertsArrayInfo); //TODO: This func prolly shouldnt be in this file. 
void openGL_bubble_sort(VertsArrayInfo*);
void openGL_selection_sort(VertsArrayInfo*);
void openGL_insertion_sort(VertsArrayInfo*);
void merge(int*, int, int, int);
void merge_sort(int*, int, int);
void merge_sort_init(int*, int);
int hoare_partition(int*, int, int);
void quick_sort(int*, int, int);
void quick_sort_init(int*, int);