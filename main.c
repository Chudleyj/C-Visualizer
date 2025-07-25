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




int main() {
	
	GLFWwindow* window = get_window(); 

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to init GLAD\n");
		return -1;
	}

	Shader shader = shader_create("vertex.glsl", "fragment.glsl");

	VertsArrayInfo vertArrInfo = get_vertArrayInfo();
	vertArrInfo.vertsArr[0] = generateArrayBarVertices(vertArrInfo);

	//openGL_bubble_sort(&vertArrInfo);
	openGL_selection_sort(&vertArrInfo);
	//openGL_insertion_sort(&vertArrInfo);
	unsigned int* VBO = malloc(vertArrInfo.totalVertArr * sizeof(unsigned int));
	if (VBO == NULL) {
		printf("VBO array malloc failure\n");
		exit(EXIT_FAILURE);
	}
	glGenBuffers(vertArrInfo.totalVertArr, VBO);

	unsigned int* VAO = malloc(vertArrInfo.totalVertArr * sizeof(unsigned int));
	if (VAO == NULL) {
		printf("VAO array malloc failure\n");
		exit(EXIT_FAILURE);
	}
	glGenVertexArrays(vertArrInfo.totalVertArr, VAO);


	for (int i = 0; i < vertArrInfo.totalVertArr; i++) {
		glBindVertexArray(VAO[i]);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);

		size_t total_floats = vertArrInfo.size_buff * 6 * 3;
		glBufferData(GL_ARRAY_BUFFER, total_floats * sizeof(float), vertArrInfo.vertsArr[i], GL_STATIC_DRAW);
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
			if (currentSortIteration >= vertArrInfo.totalVertArr) {
				currentSortIteration = 0; 
			}
			lastFrameTime = currentTime; 
		}

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader_use(shader); 		
		glBindVertexArray(VAO[currentSortIteration]);
		glDrawArrays(GL_TRIANGLES, 0, vertArrInfo.size_buff * 6);

		// glBindVertexArray(0); // no need to unbind it every time 

		glfwPollEvents();
		glfwSwapBuffers(window); 		
	}

	glDeleteVertexArrays(1, &VAO); 
	glDeleteBuffers(1, &VBO);
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