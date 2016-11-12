#include "ShaderProgram.h"

namespace gl {

ShaderProgram::ShaderProgram() {
	m_id = glCreateProgram();
}

ShaderProgram::~ShaderProgram() {
	glDeleteProgram(m_id);
}

void ShaderProgram::attach(const ShaderPtr & shader) {
	m_shaders.insert({ shader->getType(), shader });
	glAttachShader(m_id, shader->getId());
}

void ShaderProgram::compile() const {
	// Shader Program
	for (auto &pair : m_shaders) {
		pair.second->compile();
	}
	glLinkProgram(m_id);
	// Errors
	GLint success;
	GLchar infoLog[512];
	glGetProgramiv(m_id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
}

void ShaderProgram::use() const {
	glUseProgram(m_id);
}

void ShaderProgram::unbind() const {
	glUseProgram(0);
}

void ShaderProgram::setUniform(GLuint loc, glm::vec3 u3f) const {
	glUniform3f(loc, u3f.x, u3f.y, u3f.z);
}

void ShaderProgram::setUniform(GLuint loc, glm::mat3 um3fv) const {
	glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(um3fv));
}

void ShaderProgram::setUniform(GLuint loc, glm::mat4 um4fv) const {
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(um4fv));
}

void ShaderProgram::setUniform(GLuint loc, GLfloat u1f) const {
	glUniform1f(loc, u1f);
}

void ShaderProgram::setUniform(GLuint loc, GLuint u1i) const {
	glUniform1i(loc, u1i);
}

GLuint ShaderProgram::getUniformLoc(const char * name) {
	return glGetUniformLocation(m_id, name);
}

}