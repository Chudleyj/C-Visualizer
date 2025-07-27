
#include "sort.h"

//Yeah yeah yeah,  xOR swaps are not efficient in 2025 but I love it
static inline void xOR_swap(int* a, int* b) {
    *a = *a ^ *b;
    *b = *a ^ *b;
    *a = *a ^ *b;
}

void openGL_bubble_sort(VertsArrayInfo *vInfo) {
    if (vInfo->size_buff <= 1) return;
    bool swapped;
    for (int i = 0; i < vInfo->size_buff - 1; i++) {
        swapped = false;
        for (int j = 0; j < vInfo->size_buff - i - 1; j++) {
            if (*(vInfo->array + j) > *(vInfo->array + j + 1)) {
                xOR_swap((vInfo->array + j), (vInfo->array + j + 1));
                generateArrayBarVertices(vInfo, j, j+1);
                generateArrayBarVertices(vInfo, -1, -1); // Reset colors

                swapped = true;
            }
        }
        if (!swapped)break;
    }
}

void openGL_selection_sort(VertsArrayInfo *vInfo) {
    if (vInfo->size_buff <= 1) return;

    //size -1 bc last element always will be sorted automatically 
    for (int i = 0; i < vInfo->size_buff - 1; i++) {
        int min_index = i;
        for (int j = i + 1; j < vInfo->size_buff; j++) {
            if (*(vInfo->array + j) < *(vInfo->array + min_index)) {
                min_index = j;
            }
        }

        if (min_index != i) {
            xOR_swap((vInfo->array + i), (vInfo->array + min_index));

            generateArrayBarVertices(vInfo, i, min_index);
            generateArrayBarVertices(vInfo, -1, -1); // Reset colors
   
        }
    }

}

void openGL_insertion_sort(VertsArrayInfo *vInfo) {
    if (vInfo->size_buff <= 1) return;

    for (int i = 1; i < vInfo->size_buff; i++) {
        for (int j = i; j > 0; j--) {
            if (*(vInfo->array + j - 1) > *(vInfo->array + j)) {
                xOR_swap((vInfo->array + j - 1), (vInfo->array + j));
                generateArrayBarVertices(vInfo, j-1, j);     
                generateArrayBarVertices(vInfo, -1, -1); // Reset colors
            }
        }
    }
}


void openGL_merge(VertsArrayInfo *vInfo, int left, int mid, int right) {
    int lsize = mid - left + 1;
    int rsize = right - mid;


    int* l = malloc(lsize * sizeof(int)); 
    if (l == NULL) {
        printf("Mallc L Merge arr failure\n");
        exit(EXIT_FAILURE); 
    }
    
    int* r = malloc(rsize * sizeof(int));
    if (r == NULL) {
        printf("Mallc R Merge arr failure\n");
        free(l); 
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < lsize; i++) {
        l[i] = vInfo->array[left + i];
    }
    for (int i = 0; i < rsize; i++) {
        r[i] = vInfo->array[mid + 1 + i];
    }

    int i = 0, j = 0;
    for (int k = left; k <= right; k++) {
        if (i >= lsize) {
            // Left array exhausted, use right array
            //There is a fringe access invalid data edge case we need to check for or the compiler gets angry
            //This shoudlnt ever happen, but if we got sizes of 0, we never put anything in r and l, so we cant access these memory addresses
            if (rsize > 0) {
                vInfo->array[k] = r[j];
            }
            j++;
        }
        else if(j >= rsize){
            // Right array exhausted, use left array
            // Left array exhausted, use right array
            //There is a fringe access invalid data edge case we need to check for or the compiler gets angry
            //This shoudlnt ever happen, but if we got sizes of 0, we never put anything in r and l, so we cant access these memory addresses
            if (lsize > 0) {
                vInfo->array[k] = l[i];
            }
            i++; 
        }
        else if (l[i] <= r[j]) {
            vInfo->array[k] = l[i];
            i++;
        }
        else{
                vInfo->array[k] = r[j];
                j++;
            }
        generateArrayBarVertices(vInfo, j, -1);
        generateArrayBarVertices(vInfo, -1, -1); // Reset colors
    }
    free(l); 
    free(r); 
}

void openGL_merge_sort(VertsArrayInfo *vInfo, int left, int right) {
    if (left >= right)   return;
    int mid = left + (right - left) / 2;
    openGL_merge_sort(vInfo, left, mid);
    openGL_merge_sort(vInfo, mid + 1, right);
    openGL_merge(vInfo, left, mid, right);

}

void openGL_merge_sort_init(VertsArrayInfo* vInfo) {
    if (vInfo->size_buff == 1)   return;
    openGL_merge_sort(vInfo, 0, (int)vInfo->size_buff - 1);
}

int hoare_partition(VertsArrayInfo *vInfo, int low, int high) {
    int pivot = vInfo->array[low];
    int i = low - 1;
    int j = high + 1;

    while (1) {
        do {
            i++;
        } while (vInfo->array[i] < pivot);

        do {
            j--;
        } while (vInfo->array[j] > pivot);

        if (i >= j)return j;

        xOR_swap(vInfo->array + i, vInfo->array + j);
        generateArrayBarVertices(vInfo, i, j);
        generateArrayBarVertices(vInfo, -1, -1); // Reset colors
    }
}

void openGL_quick_sort(VertsArrayInfo *vInfo, int low, int high) {
    if (low >= high)   return;

    int pi = hoare_partition(vInfo, low, high);
    openGL_quick_sort(vInfo, low, pi);
    openGL_quick_sort(vInfo, pi + 1, high);

}

void openGL_quick_sort_init(VertsArrayInfo *vInfo) {
    if (vInfo->size_buff == 1) return;
    openGL_quick_sort(vInfo, 0, vInfo->size_buff - 1);
}