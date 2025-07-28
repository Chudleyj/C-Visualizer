#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct VertsArrayInfo;

void openGL_bubble_sort(struct VertsArrayInfo*);
void openGL_selection_sort(struct VertsArrayInfo*);
void openGL_insertion_sort(struct VertsArrayInfo*);
void openGL_merge(struct VertsArrayInfo*, int, int, int);
void openGL_merge_sort(struct VertsArrayInfo*, int, int);
void openGL_merge_sort_init(struct VertsArrayInfo*);
int hoare_partition(struct VertsArrayInfo*, int, int);
void openGL_quick_sort(struct VertsArrayInfo*, int, int);
void openGL_quick_sort_init(struct VertsArrayInfo*);

//Declare a type name that returns void that is a func pointer called sort_function_t that takes in int*, int
typedef void(*sort_function_t)(struct VertsArrayInfo*);
//static const void (*sort_functions[])(int*, int)={}; ==> equiv without typedef
static const sort_function_t sort_functions[] = {
	NULL,           // index 0 - unused
	openGL_bubble_sort,    // index 1
	openGL_selection_sort, // index 2
	openGL_insertion_sort, // index 3
	openGL_merge_sort_init,  // index 4
	openGL_quick_sort_init   // index 5
	// heap_sort,      // index 6
	 //radix_sort      // index 7
};
#define NUM_SORT_FUNCTIONS (sizeof(sort_functions)/ sizeof(sort_functions[0]))