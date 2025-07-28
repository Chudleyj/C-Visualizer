#include "VertsArrayInfo.h"

void generateArrayBarVertices(VertsArrayInfo* vInfo, int highlightIndex1, int highlightIndex2) {

    size_t bar_count = vInfo->size_buff;
    float bar_width = (2.0f / bar_count);// NDC is from -1 to 1 = 2.0 units

    float* vertices = NULL;
    if (bar_count > 0) {
        vertices = malloc(sizeof(float) * 6 * 6 * bar_count); // 6 vertices, 6 floats per vertex, per bar
        if (vertices == NULL) {
            printf("Array Verts Malloc failure\n");
            exit(EXIT_FAILURE);
        }
    }

    float red = 0.6588f;
    float green = 0.1961f;
    float blue = 0.1961f;
    for (int i = 0; i < bar_count; i++) {
        float x0 = -1.0f + i * bar_width;
        float x1 = x0 + bar_width * 0.8f; // add spacing (80% width)
        float y0 = -1.0f;
        float heightRatio = (float)vInfo->array[i] / vInfo->max_value;
        float y1 = -1.0f + heightRatio * 2.0f; // map [0, MAX_VALUE] --> [-1, 1]

        if (i == highlightIndex1 || i == highlightIndex2) {
            red = 1.0f;
            green = 1.0;
            blue = 1.0;
        }
        else{
            red = 0.6588f;
            green = 0.1961f;
            blue = 0.1961f;
        }
        

        float barVertices[] = {
            x0, y0, 0.0f, red, green, blue,
            x1, y0, 0.0f, red, green, blue,
            x0, y1, 0.0f, red, green, blue,

            x1, y0, 0.0f, red, green, blue,
            x1, y1, 0.0f, red, green, blue,
            x0, y1, 0.0f, red, green, blue
        };

        memcpy(vertices + i * 6 * 6, barVertices, sizeof(barVertices));
    }

    if (vInfo->totalVertArr >= vInfo->maxVertArr) {
        vInfo->maxVertArr += 100;
        float** newVertArr = realloc(vInfo->vertsArr, vInfo->maxVertArr*sizeof(float*));
        if (newVertArr == NULL) {
            printf("VertArr realloc failure\n");
            exit(EXIT_FAILURE);
        }
        vInfo->vertsArr = newVertArr;
    }

    vInfo->vertsArr[vInfo->totalVertArr] = vertices;
    vInfo->totalVertArr++; 
}

VertsArrayInfo init_vertArrayInfo() {
    int max_size = 1000;
    int min_size = 500;
    int max_val = 0;

    srand((unsigned int)time(NULL) ^ getpid());
    int size = (rand() % max_size) + 1;
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

    VertsArrayInfo vInfo = { size, max_val, arr, vertsArray, max_iterations, 0 };
    return vInfo;

}

int get_sort_choice() {
    signed char choice;
    int flush;
    printf("\nSorting Algorithm Menu:\n");
    printf("1. Bubble Sort\n");
    printf("2. Selection Sort\n");
    printf("3. Insertion Sort\n");
    printf("4. Merge Sort\n");
    printf("5. Quick Sort\n");
    //printf("6. Heap Sort\n");
    //printf("7. Radix Sort\n");
    printf("8. Exit\n");
    printf("Enter Choice: ");

    choice = getchar();

    if ((flush = getchar()) != '\n') {
        printf("Unknown Option\n");
        return -1;
    }

    int option = choice - '0';  // Convert char to int
    return option;
}

int get_vertex_draw_steps(VertsArrayInfo* vInfo) {
    generateArrayBarVertices(vInfo, -1, -1);
    int sortChoice = get_sort_choice();
    if (sortChoice == 8) {
        return -1;
    }
    else if (sortChoice >= 1 && sortChoice < NUM_SORT_FUNCTIONS) {
        sort_functions[sortChoice](vInfo);
    }
    else {
        printf("Unknown Option\n");
        return -1;
    }
}