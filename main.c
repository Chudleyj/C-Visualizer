#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h> 
#include "shader.h"
#include "sort.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height); 
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true); 
	}
}

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


GLFWwindow* get_window() {
	bool success = glfwInit();
	if (!success) {
		printf("GLFW Failed to INIT!\n"); 
		return NULL; 
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

int* get_array(int* size_buff, int *max_val) {
	int max_size = 1000;
	int min_size = 500;
	srand(time(NULL));
	size_t size = (rand() % max_size) + 1; 
	if (size < min_size) {
		size = min_size;
	}
	*size_buff = size;
	
	int* arr = malloc(size * sizeof(int)); 
	if (arr == NULL) {
		printf("Array Malloc failure\n"); 
		exit(EXIT_FAILURE); 
	}


	for (int i = 0; i < size; i++) {
		arr[i] = rand(); 
		if (arr[i] > *max_val) {
			*max_val = arr[i]; 
		}
	}

	return arr; 

}


int main() {

	GLFWwindow* window = get_window(); 

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to init GLAD\n");
		return -1;
	}

	Shader shader = shader_create("vertex.glsl", "fragment.glsl");

	size_t size_buff = 0; 
	int max_value = 0;

	int* targetArray = get_array(&size_buff, &max_value);
	//printf("%d %d", size_buff, max_value); 
	int max_iterations = (size_buff * (size_buff - 1)) / 2 + 1; // +1 for initial state, worst case sort is O(n^2), so allocate that much size. 
	float** vertsArray = malloc(max_iterations * sizeof(float*));
	if (vertsArray == NULL) {
		printf("Verts array malloc failure\n"); 
		exit(EXIT_FAILURE);
	}
	vertsArray[0] = generateArrayBarVertices(targetArray, size_buff, max_value);

	//int totalVertArrays = openGL_bubble_sort(targetArray, size_buff, vertsArray, max_value);
	int totalVertArrays = openGL_selection_sort(targetArray, size_buff, vertsArray, max_value);

	unsigned int* VBO = malloc(totalVertArrays * sizeof(unsigned int));
	if (VBO == NULL) {
		printf("VBO array malloc failure\n");
		exit(EXIT_FAILURE);
	}
	glGenBuffers(totalVertArrays, VBO);

	unsigned int* VAO = malloc(totalVertArrays * sizeof(unsigned int));
	if (VAO == NULL) {
		printf("VAO array malloc failure\n");
		exit(EXIT_FAILURE);
	}
	glGenVertexArrays(totalVertArrays, VAO);


	for (int i = 0; i < totalVertArrays; i++) {
		glBindVertexArray(VAO[i]);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);

		size_t total_floats = size_buff * 6 * 3;
		glBufferData(GL_ARRAY_BUFFER, total_floats * sizeof(float), vertsArray[i], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}
	




	int currentSortIteration = 0; 
	double lastFrameTime = glfwGetTime();
	double animationSpeed = 0.01; 
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		double currentTime = glfwGetTime(); 
		if (currentTime - lastFrameTime > animationSpeed) {
			currentSortIteration++;
			if (currentSortIteration >= totalVertArrays) {
				currentSortIteration = 0; 
			}
			lastFrameTime = currentTime; 
		}

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader_use(shader); 		
		glBindVertexArray(VAO[currentSortIteration]);
		glDrawArrays(GL_TRIANGLES, 0, size_buff * 6);

		// glBindVertexArray(0); // no need to unbind it every time 

		glfwPollEvents();
		glfwSwapBuffers(window); 		
	}

	glDeleteVertexArrays(1, &VAO); 
	glDeleteBuffers(1, &VBO);
	shader_deleteProg(shader);
	free(VAO); 
	free(VBO); 
	free(targetArray);
	for (int i = 0; i < totalVertArrays; i++) {
		free(vertsArray[i]);
	}
	free(vertsArray); 
	glfwTerminate();

	return 0; 
}