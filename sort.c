
#include "sort.h"

float* generateArrayBarVertices(VertsArrayInfo *vInfo) {

    size_t bar_count = vInfo->size_buff;
    float bar_width = (0.5f / bar_count);// NDC is from -1 to 1 = 2.0 units

    float* vertices = NULL;
    if (bar_count > 0) {
        vertices = malloc(sizeof(float) * 6 * 3 * bar_count); // 6 vertices, 3 floats per vertex, per bar
        if (vertices == NULL) {
            printf("Array Verts Malloc failure\n");
            exit(EXIT_FAILURE);
        }
    }


    for (int i = 0; i < bar_count; i++) {
        float x0 = -1.0f + i * bar_width;
        float x1 = x0 + bar_width * 0.8f; // add spacing (80% width)
        float y0 = -1.0f;
        float heightRatio = (float)vInfo->array[i] / vInfo->max_value;
        float y1 = -1.0f + heightRatio * 2.0f; // map [0, MAX_VALUE] --> [-1, 1]

        float barVertices[] = {
            x0, y0, 0.0f,
            x1, y0, 0.0f,
            x0, y1, 0.0f,

            x1, y0, 0.0f,
            x1, y1, 0.0f,
            x0, y1, 0.0f
        };

        memcpy(vertices + i * 18, barVertices, sizeof(barVertices));
    }

    vInfo->totalVertArr++; 
    return vertices;
}

VertsArrayInfo get_vertArrayInfo(){
    int max_size = 1000;
    int min_size = 500;
    int max_val = 0;
    srand(time(NULL));
    size_t size = (rand() % max_size) + 1;
    if (size < min_size) {
        size = min_size;
    }

    int* arr = malloc(size * sizeof(int));
    if (arr == NULL) {
        printf("Array Malloc failure\n");
        exit(EXIT_FAILURE);
    }


    for (int i = 0; i < size; i++) {
        arr[i] = rand();
        if (arr[i] > max_val) {
            max_val = arr[i];
        }
    }
    int max_iterations = (size * (size - 1)) / 2 + 1; // +1 for initial state, worst case sort is O(n^2), so allocate that much size. 
    float** vertsArray = malloc(max_iterations * sizeof(float*));
    if (vertsArray == NULL) {
        printf("Verts array malloc failure\n");
        exit(EXIT_FAILURE);
    }

    VertsArrayInfo vInfo = {size, max_val, arr, vertsArray, 0};
    return vInfo;

}




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
                vInfo->vertsArr[vInfo->totalVertArr] = generateArrayBarVertices(vInfo);
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
            vInfo->vertsArr[vInfo->totalVertArr] = generateArrayBarVertices(vInfo);
        }
    }

}

void openGL_insertion_sort(VertsArrayInfo *vInfo) {
    if (vInfo->size_buff <= 1) return;

    for (int i = 1; i < vInfo->size_buff; i++) {
        for (int j = i; j > 0; j--) {
            if (*(vInfo->array + j - 1) > *(vInfo->array + j)) {
                xOR_swap((vInfo->array + j - 1), (vInfo->array + j));
                vInfo->vertsArr[vInfo->totalVertArr] = generateArrayBarVertices(vInfo);
            }
        }
    }
}

/*
void merge(int* arr, int left, int mid, int right) {
    draw_array(arr, sizeof(arr) / sizeof(arr[0]));
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
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < lsize; i++) {
        l[i] = arr[left + i];
    }
    for (int i = 0; i < rsize; i++) {
        r[i] = arr[mid + 1 + i];
    }

    int i = 0, j = 0;
    for (int k = left; k <= right; k++) {
        if (i < lsize && (j >= rsize || l[i] <= r[j])) {
            arr[k] = l[i];
            i++;
        }
        else {
            arr[k] = r[j];
            j++;
        }
    }

}
*/
void merge_sort(int* arr, int left, int right) {
    if (left >= right)   return;
    int mid = left + (right - left) / 2;
    merge_sort(arr, left, mid);
    merge_sort(arr, mid + 1, right);
    //merge(arr, left, mid, right);


}

void merge_sort_init(int* arr, int size) {
    if (size == 1)   return;
    merge_sort(arr, 0, size - 1);
}

int hoare_partition(int* arr, int low, int high) {
    int pivot = arr[low];
    int i = low - 1;
    int j = high + 1;

    while (1) {
        do {
            i++;
        } while (arr[i] < pivot);

        do {
            j--;
        } while (arr[j] > pivot);

        if (i >= j)return j;

        xOR_swap(arr + i, arr + j);
    }
}

void quick_sort(int* arr, int low, int high) {
    if (low >= high)   return;

    int pi = hoare_partition(arr, low, high);
    quick_sort(arr, low, pi);
    quick_sort(arr, pi + 1, high);

}

void quick_sort_init(int* arr, int size) {
    if (size == 1) return;
    //draw_array(arr, size);
    quick_sort(arr, 0, size - 1);
   // draw_array(arr, size);
}