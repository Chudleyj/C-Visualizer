#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

float* generateArrayBarVertices(int*, size_t, unsigned int); //TODO: This func prolly shouldnt be in this file. 
unsigned int openGL_bubble_sort(int*, int, float**, unsigned int);
unsigned int openGL_selection_sort(int*, int, float**, unsigned int);
void insertion_sort(int*, int);
void merge(int*, int, int, int);
void merge_sort(int*, int, int);
void merge_sort_init(int*, int);
int hoare_partition(int*, int, int);
void quick_sort(int*, int, int);
void quick_sort_init(int*, int);