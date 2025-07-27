#include <glad/glad.h> 
#include <GLFW/glfw3.h>
 
#include "shader.h"
#include "sort.h"
#include "VertsArrayInfo.h"

//Declare a type name that returns void that is a func pointer called sort_function_t that takes in int*, int
typedef void(*sort_function_t)(VertsArrayInfo*);
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
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height); 
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true); 
	}
}

GLFWwindow* get_window() {
	bool success = glfwInit();
	if (!success) {
		printf("GLFW Failed to INIT!\n");
		return NULL;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "C-Visualizer", NULL, NULL);
	if (window == NULL)
	{
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		return NULL;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	
	return window;
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

int main() {
	int sortChoice = get_sort_choice();

	GLFWwindow* window = get_window();
	if (window == NULL) {
		return -1;
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to init GLAD\n");
		glfwTerminate();
		return -1;
	}





	Shader shader = shader_create("vertex.glsl", "fragment.glsl");

	VertsArrayInfo vertArrInfo = get_vertArrayInfo();
	generateArrayBarVertices(&vertArrInfo, -1, -1);


	
	if (sortChoice >= 1 && sortChoice < NUM_SORT_FUNCTIONS) {
		sort_functions[sortChoice](&vertArrInfo);
	}
	else {
		printf("Unknown Option\n");
		shader_deleteProg(shader);
		return 1;
	}

	unsigned int* VBO = malloc(vertArrInfo.totalVertArr * sizeof(unsigned int));
	if (VBO == NULL) {
		printf("VBO array malloc failure\n");
		shader_deleteProg(shader);
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glGenBuffers(vertArrInfo.totalVertArr, VBO);

	unsigned int* VAO = malloc(vertArrInfo.totalVertArr * sizeof(unsigned int));
	if (VAO == NULL) {
		printf("VAO array malloc failure\n");
		free(VBO);
		shader_deleteProg(shader);
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glGenVertexArrays(vertArrInfo.totalVertArr, VAO);


	for (int i = 0; i < vertArrInfo.totalVertArr; i++) {
		glBindVertexArray(VAO[i]);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);

		size_t total_floats = vertArrInfo.size_buff * 6 * 6;
		glBufferData(GL_ARRAY_BUFFER, total_floats * sizeof(float), vertArrInfo.vertsArr[i], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}
	
	int currentSortIteration = 0; 
	double lastFrameTime = glfwGetTime();
	double animationSpeed = 0.01; 

	glfwShowWindow(window);
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		double currentTime = glfwGetTime(); 
		if (currentTime - lastFrameTime > animationSpeed) {
			currentSortIteration++;
			if (currentSortIteration >= vertArrInfo.totalVertArr) {
				currentSortIteration = 0; 
			}
			lastFrameTime = currentTime; 
		}

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader_use(shader); 		
		glBindVertexArray(VAO[currentSortIteration]);
		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertArrInfo.size_buff * 6);

		glfwPollEvents();
		glfwSwapBuffers(window); 		
	}

	glDeleteVertexArrays(vertArrInfo.totalVertArr, VAO); 
	glDeleteBuffers(vertArrInfo.totalVertArr, VBO);
	shader_deleteProg(shader);
	free(VAO); 
	free(VBO); 
	free(vertArrInfo.array);
	for (int i = 0; i < vertArrInfo.totalVertArr; i++) {
		free(vertArrInfo.vertsArr[i]);
	}
	free(vertArrInfo.vertsArr);
	glfwTerminate();

	return 0; 
}