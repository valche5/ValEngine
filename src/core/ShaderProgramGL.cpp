#include "ShaderProgramGL.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "glassert.h"

ShaderGL::ShaderGL(ShaderType type, GLint coreVersion)
	: m_type(type), m_coreVersion(coreVersion), m_loadFromPaths(false)
{
	m_shader = glCreateShader((GLenum) m_type);
}

ShaderGL::~ShaderGL() {
	glDeleteShader(m_shader);
}

void ShaderGL::setCoreVersion(GLuint coreVersion) {
	m_coreVersion = coreVersion;
}

void ShaderGL::setDefines(const std::vector<std::string>& defines) {
	m_defines = defines;
}

void ShaderGL::setSourceFile(const std::string & path) {
	m_loadFromPaths = true;
	m_paths.clear();
	m_paths.push_back(path);
}

void ShaderGL::setSourceFiles(const std::vector<std::string>& paths) {
	m_loadFromPaths = true;
	m_paths = paths;

}

void ShaderGL::setSource(const std::string & source) {
	m_loadFromPaths = false;
	m_source = source;
}

void ShaderGL::setSources(const std::vector<std::string>& sources) {
	std::string finalSource;
	for (auto &source : sources) {
		finalSource += source;
	}
	m_source = finalSource;
}

GLuint ShaderGL::compile() const {
	std::string defines;

	defines += "#version " + std::to_string(m_coreVersion) + " core\n";
	for (auto define : m_defines) {
		defines += "#define " + define + "\n";
	}

	std::string code = defines + "\n";
	if (m_loadFromPaths) {
		for (auto path : m_paths) {
			code += util::readFile(path);
		}
	} else {
		code = m_source;
	}

	const char *shaderCode = code.c_str();
	glShaderSource(m_shader, 1, &shaderCode, NULL);
	glCompileShader(m_shader);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(m_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(m_shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	return m_shader;
}

ShaderType ShaderGL::getType() const {
	return m_type;
}

GLuint ShaderGL::glId() {
	return m_shader;
}

ShaderProgramGL::ShaderProgramGL() {
	m_program = glCreateProgram();
}

ShaderProgramGL::~ShaderProgramGL() {
	glDeleteProgram(m_program);
}

void ShaderProgramGL::attach(const ShaderGLPtr & shader) {
	m_shaders.insert({ shader->getType(), shader });
	glAttachShader(m_program, shader->glId());
}

void ShaderProgramGL::compile() const {
	// Shader Program
	for (auto &pair : m_shaders) {
		pair.second->compile();
	}
	glLinkProgram(m_program);
	// Errors
	GLint success;
	GLchar infoLog[512];
	glGetProgramiv(m_program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
}

void ShaderProgramGL::use() const {
	glUseProgram(m_program);
}

void ShaderProgramGL::setUniform(GLuint loc, glm::vec3 u3f) const
{
	glUniform3f(loc, u3f.x, u3f.y, u3f.z);
}

void ShaderProgramGL::setUniform(GLuint loc, glm::mat3 um3fv) const
{
	glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(um3fv));
}

void ShaderProgramGL::setUniform(GLuint loc, glm::mat4 um4fv) const
{
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(um4fv));
}

void ShaderProgramGL::setUniform(GLuint loc, GLfloat u1f) const
{
	glUniform1f(loc, u1f);
}

void ShaderProgramGL::setUniform(GLuint loc, GLuint u1i) const
{
	glUniform1i(loc, u1i);
}