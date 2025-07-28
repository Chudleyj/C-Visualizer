#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <sys/types.h>
#include "sort.h"

typedef struct VertsArrayInfo {
	int32_t size_buff; //We are gonna need to cast this into a GLsizei, which is int32, so just use that now instead of size_t.
	int max_value;
	int* array;
	float** vertsArr;
	size_t maxVertArr;
	int32_t totalVertArr; //We are gonna need to cast this into a GLsizei, which is int32, so just use that now instead of size_t.
}VertsArrayInfo;

VertsArrayInfo init_vertArrayInfo();

void generateArrayBarVertices(VertsArrayInfo*, int, int); 
int get_vertex_draw_steps(VertsArrayInfo*);
int get_sort_choice(); 