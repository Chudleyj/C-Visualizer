
#include "sort.h"

float* generateArrayBarVertices(int *values, size_t bar_count, unsigned int max_value) {

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
        float heightRatio = (float)values[i] / max_value;
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

    return vertices;
}


//Yeah yeah yeah,  xOR swaps are not efficient in 2025 but I love it
static inline void xOR_swap(int* a, int* b) {
    *a = *a ^ *b;
    *b = *a ^ *b;
    *a = *a ^ *b;
}

unsigned int bubble_sort(int* arr, int size, float **vertsArray, unsigned int max_val) {
    unsigned int vertsArrayCounter = 1; //start at 1 bc we do the first index b4 we sort 
    if (size <= 1) return;
    bool swapped;
    for (int i = 0; i < size - 1; i++) {
        swapped = false;
        for (int j = 0; j < size - i - 1; j++) {
            if (*(arr + j) > *(arr + j + 1)) {
                xOR_swap((arr + j), (arr + j + 1));
                vertsArray[vertsArrayCounter++] = generateArrayBarVertices(arr, size, max_val);
                swapped = true;

               // draw_array(arr, size);
            }
        }
        if (!swapped)break;
    }

    return vertsArrayCounter;
}

void selection_sort(int* arr, int size) {

    if (size <= 1) return;

    //size -1 bc last element always will be sorted automatically 
    for (int i = 0; i < size - 1; i++) {
        int min_index = i;
        for (int j = i + 1; j < size; j++) {
            if (*(arr + j) < *(arr + min_index)) {
                min_index = j;
            }
        }

        if (min_index != i) {
            xOR_swap((arr + i), (arr + min_index));
        }
    }
}

void insertion_sort(int* arr, int size) {
    if (size <= 1) return;

    for (int i = 1; i < size; i++) {
        for (int j = i; j > 0; j--) {
            if (*(arr + j - 1) > *(arr + j)) {
                xOR_swap((arr + j - 1), (arr + j));
            }
        }
    }
}

/*
void merge(int* arr, int left, int mid, int right) {
    draw_array(arr, sizeof(arr) / sizeof(arr[0]));
    int lsize = mid - left + 1;
    int rsize = right - mid;
    int l[lsize];
    int r[rsize];

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
   // merge(arr, left, mid, right);


}

void merge_sort_init(int* arr, int size) {
    if (size == 1)   return;
    merge_sort(arr, 0, size - 1);
   // draw_array(arr, size);
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