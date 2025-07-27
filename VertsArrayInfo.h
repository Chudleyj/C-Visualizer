#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <sys/types.h>

typedef struct VertsArrayInfo {//TODO: This func prolly shouldnt be in this file. 
    int32_t size_buff; //We are gonna need to cast this into a GLsizei, which is int32, so just use that now instead of size_t.
    int max_value;
    int* array;
    float** vertsArr;
    //I think i need some kinda arr to track the size of EACH vertex arr instance
    size_t maxVertArr; 
    int32_t totalVertArr; //We are gonna need to cast this into a GLsizei, which is int32, so just use that now instead of size_t.
}VertsArrayInfo;

VertsArrayInfo get_vertArrayInfo();//TODO: This func prolly shouldnt be in this file. 

void generateArrayBarVertices(VertsArrayInfo*, int, int); //TODO: This func prolly shouldnt be in this file. 