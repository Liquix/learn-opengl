#ifndef SHADER_H
#define SHADER_H

#include <GLAD\glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	unsigned ID;	// Shader program ID

	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);

	void use();		// Activate shader program

	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
};

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	// Load from files, print error if load fails
	std::string vertexStr, fragmentStr;
	std::ifstream vShaderFile, fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexStr = vShaderStream.str();
		fragmentStr = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "Fatal error: Could not find shader file " << vertexPath << " or " << fragmentPath << "." << std::endl;
		return;
	}

	const char* vShaderSrc = vertexStr.c_str();
	const char* fShaderSrc = fragmentStr.c_str();

	//std::cout << "Successfully loaded vertex shader from " << vertexPath << "!" << std::endl << "Successfully loaded fragment shader from " << fragmentPath << "!" << std::endl;

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
	}
	else	std::cout << "Compiled vertex shader: " << vertexPath << std::endl;
	if (!fSuccess) {
		glGetShaderInfoLog(fShader, 512, NULL, info);
		std::cout << "Fragment shader compilation failed: " << info << std::endl;
	}
	else	std::cout << "Compiled fragment shader: " << fragmentPath << std::endl << std::endl;

	// Link shaders into program
	ID = glCreateProgram();
	glAttachShader(ID, vShader);
	glAttachShader(ID, fShader);
	glLinkProgram(ID);

	// Check for shader link errors
	vSuccess = 0;
	glGetProgramiv(ID, GL_LINK_STATUS, &vSuccess);
	if (!vSuccess) {
		glGetProgramInfoLog(ID, 512, NULL, info);
		std::cout << "Shader program linking failed: " << info << std::endl;
	}
	//else std::cout << "Shader program successfully linked!" << std::endl;

	// Once shaders are linked into program, we can delete
	glDeleteShader(vShader);
	glDeleteShader(fShader);
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), (float)value);
}

#endif