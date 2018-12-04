#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <gl\GL.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Shader.h"
#include "stb_image.h"

#define X_RESOLUTION 640
#define Y_RESOLUTION 480

std::string vShaderName = "FirstVertex.vert";
std::string fShaderName = "Mandelbrot.frag";
float zoom = 1.5f;

// Load shaders
Shader *shaderProg = nullptr;

void handleError(int errorCode, const char* errorDescription)
{
	std::cout << "GLFW Error! Code: " << errorCode << ". Description: " << errorDescription << std::endl;
}

void windowResizedCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void keypressCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		GLint pMode[2];
		glGetIntegerv(GL_POLYGON_MODE, pMode);
		if (pMode[0] == GL_FILL)	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		if (pMode[0] == GL_LINE)	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		glDeleteProgram(shaderProg->ID);
		delete shaderProg;
		shaderProg = new Shader(vShaderName.c_str(), fShaderName.c_str());
	}
}

void mouseClickCallback(GLFWwindow *window, int button, int action, int mods)
{
	double xMouse, yMouse;
	glfwGetCursorPos(window, &xMouse, &yMouse);
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		std::cout << "Click: (" << xMouse / X_RESOLUTION << ", " << yMouse / Y_RESOLUTION << ")" << std::endl;
		//shaderProg->setVec2("center", (float)xMouse / X_RESOLUTION, (float)yMouse / Y_RESOLUTION);
	}
}

void mouseScrollCallback(GLFWwindow *window, double xOffset, double yOffset)
{
	zoom += -(float)yOffset * 0.05f;
	shaderProg->setFloat("scale", zoom);
}

int main(int argc, char **argv)
{
	// Initialize GLFW
	glfwSetErrorCallback(handleError);
	if (!glfwInit())
	{
		std::cout << "Error: glfwInit() failed" << std::endl;
		std::cin.get();
		glfwTerminate();
		return -1;
	}

	// Window setup
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(X_RESOLUTION, Y_RESOLUTION, "Learn OpenGL - Mandelbrot Fragment Shader", NULL, NULL);
	if (!window)
	{
		std::cout << "Error: glfwCreateWindow() failed" << std::endl;
		std::cin.get();
		glfwTerminate();
		return -2;
	}
	glfwSetWindowPos(window, 2000, 100);
	glfwMakeContextCurrent(window);

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Error: gladLoadGLLoader() failed" << std::endl;
		return -3;
	}

	// Init success! Print library info
	std::cout << "OpenGL version:\t" << glGetString(GL_VERSION) << std::endl
		<< "GLFW version:\t" << glfwGetVersionString() << std::endl
		<< "\nPress W to toggle wireframe mode." << std::endl
		<< "Press R to hot reload shaders." << std::endl
		<< "Press ESC to close OpenGL window.\n" << std::endl;

	// Now we can start calling OpenGL functions. Set up viewport
	glViewport(0, 0, X_RESOLUTION, Y_RESOLUTION);
	glfwSetFramebufferSizeCallback(window, windowResizedCallback);

	// Input
	glfwSetKeyCallback(window, keypressCallback);
	glfwSetMouseButtonCallback(window, mouseClickCallback);
	glfwSetScrollCallback(window, mouseScrollCallback);
	
	shaderProg = new Shader(vShaderName.c_str(), fShaderName.c_str());
	shaderProg->use();
	shaderProg->setInt("max_iterations", 50);
	shaderProg->setFloat("scale", zoom);

	// Vertex data for two triangles forming a square.
	float vertices[] = {
		// Position
		1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f
	};

	// Define two triangles based on shared vertices
	unsigned indices[] = {
		0, 1, 3,	// First triangle (top right, bottom right, top left)
		1, 2, 3		// Second triangle (bottom right, bottom left, top left)
	};

	// Vertex array object - holds all below calls
	unsigned vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Vertex buffer object - efficiently store vertex data in OpenGL-friendly format
	// Note: this needs to happen before glVertexAttribPointer() call below
	unsigned vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Element buffer object - stores indexes to avoid reusing vertexes
	unsigned ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	/**** glVertexAttribPointer() - Define how to interpret vertex data
	0				  attribute to configure - 0 is position, 1 is color, 2 is texture
	3				  elements per vertex
	GL_FLOAT		  datatype of 3 elements
	GL_FALSE		  do not normalize
	3 * sizeof(float) size in bytes of each vertex
	(void*)0		  offset- where the first chunk begins relative to beginning of array  ****/
	// Note: we can only call this after binding VBO above
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	std::cout << "Entering render loop..." << std::endl;

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		shaderProg->use();

		// Update mandelbrot scale and center
		float timeVal = (float) glfwGetTime();
		float greenVal = (sin(timeVal) / 2.0f) + 1.0f;

		
		//shaderProg->setFloat("scale", greenVal);
		
		//shaderProg->setVec2("center", 0, 0);

		glBindVertexArray(vao);		// This also binds attached EBO

		// Clear background to cornflower blue
		glClearColor(100.0f / 255.0f, 149.0f / 255.0f, 237.0f / 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw triangle from EBO bound to ELEMENT_ARRAY_BUFFER
		// glDrawElements(mode, numVertices, indexDataType, offset)
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);	// ?????

		// Process events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup
	glfwTerminate();
	return 0;
}