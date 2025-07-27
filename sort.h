#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "VertsArrayInfo.h"

void openGL_bubble_sort(VertsArrayInfo*);
void openGL_selection_sort(VertsArrayInfo*);
void openGL_insertion_sort(VertsArrayInfo*);
void openGL_merge(VertsArrayInfo*, int, int, int);
void openGL_merge_sort(VertsArrayInfo*, int, int);
void openGL_merge_sort_init(VertsArrayInfo*);
int hoare_partition(VertsArrayInfo*, int, int);
void openGL_quick_sort(VertsArrayInfo*, int, int);
void openGL_quick_sort_init(VertsArrayInfo*);