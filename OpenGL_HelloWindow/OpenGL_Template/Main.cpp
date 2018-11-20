#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <gl\GL.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define X_RESOLUTION 640
#define Y_RESOLUTION 480

std::string vShaderName = "FirstVertex.vert";
std::string fShaderName = "FirstFrag.frag";

std::string loadShaderSrc(std::string fileName)
{
	std::ifstream tmpFile(fileName, std::ios::binary);
	std::stringstream sstr;

	if(tmpFile.good())	sstr << tmpFile.rdbuf() << "\n\0";
	else return "";

	std::string tmpStr = sstr.str();
	tmpFile.close();
	return tmpStr;
}

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

	GLFWwindow* window = glfwCreateWindow(X_RESOLUTION, Y_RESOLUTION, "Learn OpenGL - Hello Window", NULL, NULL);
	if (!window)
	{
		std::cout << "Error: glfwCreateWindow() failed" << std::endl;
		std::cin.get();
		glfwTerminate();
		return -2;
	}
	glfwMakeContextCurrent(window);

	// GLAD (OpenGL function pointer loader) init
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Error: gladLoadGLLoader() failed" << std::endl;
		return -3;
	}

	// Init success! Print library info
	std::cout << "Initialization successful!" << std::endl 
		<< "OpenGL version info:\t" << glGetString(GL_VERSION) << std::endl
		<< "GLFW version info:\t" << glfwGetVersionString() << std::endl
		<< "\nPress W to toggle wireframe mode." << std::endl
		<< "Press ESC to close OpenGL window.\n" << std::endl;

	// Now we can start calling OpenGL functions. Set up viewport
	glViewport(0, 0, X_RESOLUTION, Y_RESOLUTION);
	glfwSetFramebufferSizeCallback(window, windowResizedCallback);

	// Input
	glfwSetKeyCallback(window, keypressCallback);
	
	// Load shaders
	std::string vShaderStr = loadShaderSrc(vShaderName);
	const char * vShaderSrc = vShaderStr.c_str();
	std::string fShaderStr = loadShaderSrc(fShaderName);
	const char * fShaderSrc = fShaderStr.c_str();

	// Compile shaders
	unsigned vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	fShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vShader, 1, &vShaderSrc, NULL);
	glCompileShader(vShader);
	glShaderSource(fShader, 1, &fShaderSrc, NULL);
	glCompileShader(fShader);

	// Print compilation errors if applicable
	int vSuccess, fSuccess = 0;
	char info[512];
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &vSuccess);
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &fSuccess);
	if (!vSuccess) {
		glGetShaderInfoLog(vShader, 512, NULL, info);
		std::cout << "Vertex shader compilation failed: " << info << std::endl;
	} else	std::cout << "Vertex shader successfully compiled!" << std::endl;
	if (!fSuccess) {
		glGetShaderInfoLog(fShader, 512, NULL, info);
		std::cout << "Fragment shader compilation failed: " << info << std::endl;
	} else	std::cout << "Fragment shader successfully compiled!" << std::endl;

	// Link shaders
	unsigned shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vShader);
	glAttachShader(shaderProgram, fShader);
	glLinkProgram(shaderProgram);

	// Check for shader link errors
	vSuccess = 0;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &vSuccess);
	if (!vSuccess) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, info);
		std::cout << "Shader program linking failed: " << info << std::endl;
	} else std::cout << "Shader program successfully linked!" << std::endl;

	// Once shaders are linked into program, we can delete
	glDeleteShader(vShader);
	glDeleteShader(fShader);

	// Vertex data for two triangles forming a square.
	// All vertexes are shared except the two corners opposite the hypotenuse.
	float vertices[] = {
		0.5f, 0.5f, 0.0f,	// Top right
		0.5f, -0.5f, 0.0f,	// Bottom right
		-0.5f, -0.5f, 0.0f,	// Bottom left
		-0.5f, 0.5f, 0.0f,	// Top left
		0.0f, 0.75f, 0.0f	// Top center
	};

	// Define two triangles based on shared vertices
	unsigned indices[] = {
		0, 1, 3,	// First triangle (top right, bottom right, top left)
		1, 2, 3,	// Second triangle (bottom right, bottom left, top left)
		3, 4, 0		// Third triangle (tip)
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
	0				  attribute to configure - simple shader only has element 0 (position)
	3				  elements per vertex
	GL_FLOAT		  datatype of 3 elements
	GL_FALSE		  do not normalize
	3 * sizeof(float) size in bytes of each vertex
	(void*)0		  offset, beginning of data in vertex array  ****/
	// Note: we can only call this after binding VBO above
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	
	std::cout << "Entering render loop..." << std::endl;

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		// Input processing (automatically handled by callback - no call in render loop)

		// Set up to draw triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(vao);		// This also binds attached EBO

		// Clear background to cornflower blue
		glClearColor(100.0f / 255.0f, 149.0f / 255.0f, 237.0f / 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw triangle from EBO bound to ELEMENT_ARRAY_BUFFER
		// glDrawElements(mode, numVertices, indexDataType, offset)
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);	// ?????

		// Process events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup
	glfwTerminate();
	return 0;
}