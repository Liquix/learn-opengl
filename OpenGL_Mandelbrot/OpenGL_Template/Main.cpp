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

#define X_RESOLUTION 720
#define Y_RESOLUTION 720

GLfloat colorData[] = {
	222,119,111,
	224,119,115,
	225,120,118,
	226,120,122,
	227,121,126,
	228,122,129,
	229,122,133,
	229,123,137,
	230,124,141,
	230,125,144,
	230,126,148,
	230,127,152,
	230,129,156,
	229,130,160,
	229,131,163,
	228,133,167,
	227,135,171,
	226,136,175,
	225,138,178,
	223,140,182,
	222,142,185,
	220,144,189,
	218,146,192,
	216,148,196,
	214,150,199,
	211,152,202,
	209,154,205,
	206,156,208,
	203,158,211,
	200,161,214,
	197,163,217,
	194,165,219,
	190,167,222,
	187,170,224,
	183,172,226,
	179,174,228,
	175,176,230,
	171,179,232,
	167,181,233,
	162,183,234,
	158,185,236,
	154,187,237,
	149,189,238,
	144,192,238,
	140,194,239,
	135,196,239,
	130,198,240,
	126,200,240,
	121,202,239,
	116,204,239,
	111,205,239,
	107,207,238,
	102,209,237,
	98,211,236,
	94,213,235,
	90,214,234,
	86,216,232,
	82,218,231,
	79,219,229,
	77,221,227,
	75,222,225,
	73,224,223,
	72,225,221,
	72,227,218,
	72,228,216,
	73,229,213,
	74,231,210,
	76,232,207,
	79,233,204,
	82,234,201,
	85,235,198,
	89,236,195,
	93,237,192,
	97,238,188,
	102,239,185,
	107,240,182,
	111,241,178,
	116,242,175,
	121,243,171,
	127,243,168,
	132,244,164,
	137,245,161,
	143,245,157,
	148,246,154,
	153,246,151,
	159,247,147,
	164,247,144,
	170,247,141,
	176,248,138,
	181,248,134,
	187,248,131,
	193,248,129,
	198,248,126,
	204,248,123,
	210,248,121,
	216,248,118,
	221,248,116,
	227,248,114,
	233,248,112,
	239,247,111,
	233,248,112,
	227,248,114,
	221,248,116,
	216,248,118,
	210,248,121,
	204,248,123,
	198,248,126,
	193,248,129,
	187,248,131,
	181,248,134,
	176,248,138,
	170,247,141,
	164,247,144,
	159,247,147,
	153,246,151,
	148,246,154,
	143,245,157,
	137,245,161,
	132,244,164,
	127,243,168,
	121,243,171,
	116,242,175,
	111,241,178,
	107,240,182,
	102,239,185,
	97,238,188,
	93,237,192,
	89,236,195,
	85,235,198,
	82,234,201,
	79,233,204,
	76,232,207,
	74,231,210,
	73,229,213,
	72,228,216,
	72,227,218,
	72,225,221,
	73,224,223,
	75,222,225,
	77,221,227,
	79,219,229,
	82,218,231,
	86,216,232,
	90,214,234,
	94,213,235,
	98,211,236,
	102,209,237,
	107,207,238,
	111,205,239,
	116,204,239,
	121,202,239,
	126,200,240,
	130,198,240,
	135,196,239,
	140,194,239,
	144,192,238,
	149,189,238,
	154,187,237,
	158,185,236,
	162,183,234,
	167,181,233,
	171,179,232,
	175,176,230,
	179,174,228,
	183,172,226,
	187,170,224,
	190,167,222,
	194,165,219,
	197,163,217,
	200,161,214,
	203,158,211,
	206,156,208,
	209,154,205,
	211,152,202,
	214,150,199,
	216,148,196,
	218,146,192,
	220,144,189,
	222,142,185,
	223,140,182,
	225,138,178,
	226,136,175,
	227,135,171,
	228,133,167,
	229,131,163,
	229,130,160,
	230,129,156,
	230,127,152,
	230,126,148,
	230,125,144,
	230,124,141,
	229,123,137,
	229,122,133,
	228,122,129,
	227,121,126,
	226,120,122,
	225,120,118,
	224,119,115,
	222,119,111 };

std::string vShaderName = "FirstVertex.vert";
std::string fShaderName = "Mandelbrot.frag";
float zoom = 1.5f;
unsigned maxIterations = 50;
float xCenter = 0.0f;

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
		shaderProg->use();
		shaderProg->setInt("max_iterations", maxIterations);
		shaderProg->setFloat("scale", zoom);
		glUniform3fv(glGetUniformLocation(shaderProg->ID, "colors"), 600, colorData);
	}
	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
	{
		maxIterations += 10;
		std::cout << "Iteration depth increased to " << maxIterations << "." << std::endl;
		shaderProg->setInt("max_iterations", maxIterations);
	}
	if (key == GLFW_KEY_E && action == GLFW_PRESS)
	{
		maxIterations -= 10;
		std::cout << "Iteration depth decreased to " << maxIterations << "." << std::endl;
		shaderProg->setInt("max_iterations", maxIterations);
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{
		xCenter += 0.05f;
		shaderProg->setVec2("center", xCenter, 0.0f);
		std::cout << "Moving left horizontally, new X: " << xCenter << std::endl;
	}
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
		xCenter -= 0.05f;
		shaderProg->setVec2("center", xCenter, 0.0f);
		std::cout << "Moving right horizontally, new X: " << xCenter << std::endl;
	}
}

void mouseClickCallback(GLFWwindow *window, int button, int action, int mods)
{
	double xMouse, yMouse;
	glfwGetCursorPos(window, &xMouse, &yMouse);
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		std::cout << "Click: (" << xMouse << ", " << yMouse << ")" << std::endl;
		//double mouseRe = (double)xMouse / (X_RESOLUTION / (1 - (-2))) + (-2);
		//double mouseIm = ((double)yMouse / (Y_RESOLUTION / (1 - (-1))) + (-1)) * -1;

		float mouseRe = (float)xMouse * zoom * (3.0f / (float)X_RESOLUTION) - 2.0f;
		float mouseIm = (float)yMouse * (2.0f / (float)Y_RESOLUTION) - 1.0f;

		//mouseRe += xCenter;

		//realX = xMouse * (3 / 640) - 2
		//realY = yMouse * (2 / 480) - 1

		//std::cout << "Click: (" << mouseRe << ", " << -mouseIm << ")" << std::endl;
		
		shaderProg->setVec2("center", (float)xMouse, (float)yMouse);
	}
}

void mouseScrollCallback(GLFWwindow *window, double xOffset, double yOffset)
{
	if (yOffset > 0.0)
	{
		zoom /= 1.05f;
	}
	if (yOffset < 0.0)
	{
		zoom *= 1.05f;
	}
	//zoom += -(float)yOffset;
	shaderProg->setFloat("scale", zoom);

	std::cout << "Changing zoom level to " << zoom << std::endl;
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
	//glfwSetWindowPos(window, 2000, 100);
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
		<< "Use the mouse wheel to zoom in and out." << std::endl
		<< "Press Q or W to increase or decrease max iterations." << std::endl
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
	shaderProg->setInt("max_iterations", maxIterations);
	shaderProg->setFloat("scale", zoom);
	xCenter = -0.5f;
	//shaderProg->setVec2("center", xCenter, 0.0f);
	
	glUniform3fv(glGetUniformLocation(shaderProg->ID, "colors"), 600, &colorData[0]);

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
		float t = (float)glfwGetTime() / 16.0f;
		float tDec = t - (int)(t);
		float x = (int)(sin((int)(t) % 3 + 1.0f + tDec) * (float)X_RESOLUTION) % (X_RESOLUTION / 2);
		//float y = (int)((sin(t + 1.0f)) * (float)Y_RESOLUTION) % Y_RESOLUTION;
		float y = x * 2;

		std::cout << x << std::endl;

		shaderProg->setVec2("center", x, y);

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