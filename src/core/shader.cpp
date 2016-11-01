#include "shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "glassert.h"

Shader::Shader() {
}

Shader::~Shader() {
	//glDeleteProgram(m_program);
}

void Shader::load(const std::string &vertexPath, const std::vector<std::string> &fragmentPaths, const std::vector<std::string> &defines) {
	m_defines = defines;
	m_vertexPath = vertexPath;
	m_fragmentPaths = fragmentPaths;
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
		std::stringstream vShaderStream, fShaderStream;

		vShaderFile.open(directory + m_vertexPath);
		vShaderStream << vShaderFile.rdbuf();
		vShaderFile.close();

		for (auto fragmentPath : m_fragmentPaths) {
			fShaderFile.open(directory + fragmentPath);
			fShaderStream << fShaderFile.rdbuf();
			fShaderFile.close();
		}

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
	glAssert(m_program = glCreateProgram());
	glAssert(glAttachShader(m_program, vertex));
	glAssert(glAttachShader(m_program, fragment));
	glAssert(glLinkProgram(m_program));
	// Errors
	glAssert(glGetProgramiv(m_program, GL_LINK_STATUS, &success));
	if (!success) {
		glAssert(glGetProgramInfoLog(m_program, 512, NULL, infoLog));
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// Delete the shaders as they're no longer necessary
	glAssert(glDeleteShader(vertex));
	glAssert(glDeleteShader(fragment));
}

void Shader::reload()
{
	glAssert(glDeleteProgram(m_program));
    load();
}

void Shader::use() const {
	glAssert(glUseProgram(m_program));
}

void Shader::setUniform(GLuint loc, glm::vec3 u3f) const
{
	glUniform3f(loc, u3f.x, u3f.y, u3f.z);
}

void Shader::setUniform(GLuint loc, glm::mat3 um3fv) const
{
	glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(um3fv));
}

void Shader::setUniform(GLuint loc, glm::mat4 um4fv) const
{
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(um4fv));
}

void Shader::setUniform(GLuint loc, GLfloat u1f) const
{
	glUniform1f(loc, u1f);
}

void Shader::setUniform(GLuint loc, GLuint u1i) const
{
	glUniform1i(loc, u1i);
}
