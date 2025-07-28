#include <glad/glad.h> 
#include <GLFW/glfw3.h>
 
#include "shader.h"
#include "sort.h"
#include "VertsArrayInfo.h"


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
bool init_glfw();
GLFWwindow* get_window();
void config_vertex_objs(VertsArrayInfo vInfo, unsigned int* VBO, unsigned int* VAO);
void draw_loop(GLFWwindow* window, VertsArrayInfo vInfo, unsigned int* VAO, Shader shader);
int render_scene(GLFWwindow* window);


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height); 
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true); 
	}
}

bool init_glfw() {
	bool success = glfwInit();
	if (!success) {
		printf("GLFW Failed to INIT!\n");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

	return 	success;
}

GLFWwindow* get_window() {
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


void config_vertex_objs(VertsArrayInfo vInfo, unsigned int* VBO, unsigned int* VAO){
	glGenBuffers(vInfo.totalVertArr, VBO);
	glGenVertexArrays(vInfo.totalVertArr, VAO);


	for (int i = 0; i < vInfo.totalVertArr; i++) {
		glBindVertexArray(VAO[i]);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);

		size_t total_floats = vInfo.size_buff * 6 * 6;
		glBufferData(GL_ARRAY_BUFFER, total_floats * sizeof(float), vInfo.vertsArr[i], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}
}

void draw_loop(GLFWwindow* window, VertsArrayInfo vInfo, unsigned int* VAO, Shader shader) {
	int currentSortIteration = 0;
	double lastFrameTime = glfwGetTime();
	double animationSpeed = 0.01;
	glfwShowWindow(window);
	glfwGetWindowAttrib(window, GLFW_VISIBLE);
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		double currentTime = glfwGetTime();
		if (currentTime - lastFrameTime > animationSpeed) {
			currentSortIteration++;
			if (currentSortIteration >= vInfo.totalVertArr) {
				glfwHideWindow(window); 
				break;
			}
			lastFrameTime = currentTime;
		}

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader_use(shader);
		glBindVertexArray(VAO[currentSortIteration]);
		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vInfo.size_buff * 6);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}
}


int render_scene(GLFWwindow* window) {
	VertsArrayInfo vertArrInfo = init_vertArrayInfo();
	if (get_vertex_draw_steps(&vertArrInfo) == -1) {
		return -1; 
	};

	unsigned int* VBO = malloc(vertArrInfo.totalVertArr * sizeof(unsigned int));
	if (VBO == NULL) {
		printf("VBO array malloc failure\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	unsigned int* VAO = malloc(vertArrInfo.totalVertArr * sizeof(unsigned int));
	if (VAO == NULL) {
		printf("VAO array malloc failure\n");
		free(VBO);
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	config_vertex_objs(vertArrInfo, VBO, VAO);

	Shader shader = shader_create("vertex.glsl", "fragment.glsl");
	draw_loop(window, vertArrInfo, VAO, shader);

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

	return 0; 
}

int main() {
	bool glfwSuccess = init_glfw(); 
	if (!glfwSuccess) {
		return -1; 
	}

	GLFWwindow* window = get_window();
	if (window == NULL) {
		glfwTerminate();
		return -1;
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to init GLAD\n");
		glfwTerminate();
		return -1;
	}
	int continueStatus = 0; 
	while (continueStatus == 0) {
		continueStatus = render_scene(window);
	}
	glfwDestroyWindow(window); 
	glfwTerminate();
	return 0; 
}