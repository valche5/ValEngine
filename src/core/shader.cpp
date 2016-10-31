#include "shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <glassert.h>

Shader::Shader() {
}

Shader::~Shader() {
	glDeleteProgram(program);
}

void Shader::load(char const *vertexPath, char const *fragmentPath, const std::vector<std::string> &defines) {
	m_defines = defines;
	m_vertexPath = vertexPath;
	m_fragmentPath = fragmentPath;
	load();
}

void Shader::load()
{
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::badbit);
	try {
		vShaderFile.open(m_vertexPath);
		fShaderFile.open(m_fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		vShaderFile.close();
		fShaderFile.close();
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

	} catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULY_READ" << std::endl;
	}

	for (auto define : m_defines) {
		fragmentCode = "#define " + define + "\n" + fragmentCode;
	}
	fragmentCode = "#version 330 core\n" + fragmentCode;

	const char * vShaderCode = vertexCode.c_str();
	const char * fShaderCode = fragmentCode.c_str();

	// Vertex Shader
	GLint vertex = glCreateShader(GL_VERTEX_SHADER);
	glAssert(glShaderSource(vertex, 1, &vShaderCode, NULL));

	// Fragment Shader
	GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glAssert(glShaderSource(fragment, 1, &fShaderCode, NULL));

	GLint success;
	GLchar infoLog[512];

	// Vertex Shader
	glAssert(glCompileShader(vertex));
	// Errors
	glAssert(glGetShaderiv(vertex, GL_COMPILE_STATUS, &success));
	if (!success) {
		glAssert(glGetShaderInfoLog(vertex, 512, NULL, infoLog));
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Fragment Shader
	glAssert(glCompileShader(fragment));
	// Errors
	glAssert(glGetShaderiv(fragment, GL_COMPILE_STATUS, &success));
	if (!success) {
		glAssert(glGetShaderInfoLog(fragment, 512, NULL, infoLog));
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Shader Program
	glAssert(program = glCreateProgram());
	glAssert(glAttachShader(program, vertex));
	glAssert(glAttachShader(program, fragment));
	glAssert(glLinkProgram(program));
	// Errors
	glAssert(glGetProgramiv(program, GL_LINK_STATUS, &success));
	if (!success) {
		glAssert(glGetProgramInfoLog(program, 512, NULL, infoLog));
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// Delete the shaders as they're no longer necessary
	glAssert(glDeleteShader(vertex));
	glAssert(glDeleteShader(fragment));
}

void Shader::reload()
{
	glAssert(glDeleteProgram(program));
    load();
}

void Shader::use() {
	glAssert(glUseProgram(program));
}

void Shader::setUniform(const std::string &loc, glm::vec3 u3f)
{
	glAssert(glUniform3f(glGetUniformLocation(program, loc.c_str()), u3f.x, u3f.y, u3f.z));
}

void Shader::setUniform(const std::string & loc, glm::mat3 um3fv)
{
	glAssert(glUniformMatrix3fv(glGetUniformLocation(program, loc.c_str()), 1, GL_FALSE, glm::value_ptr(um3fv)));
}

void Shader::setUniform(const std::string &loc, glm::mat4 um4fv)
{
	glAssert(glUniformMatrix4fv(glGetUniformLocation(program, loc.c_str()), 1, GL_FALSE, glm::value_ptr(um4fv)));
}

void Shader::setUniform(const std::string &loc, GLfloat u1f)
{
	glAssert(glUniform1f(glGetUniformLocation(program, loc.c_str()), u1f));
}

void Shader::setUniform(const std::string &loc, GLuint u1i)
{
	glAssert(glUniform1i(glGetUniformLocation(program, loc.c_str()), u1i));
}
