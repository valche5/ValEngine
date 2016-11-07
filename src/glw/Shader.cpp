#include "Shader.h"

#include <string>

#include <utils/File.h>

namespace gl {

Shader::Shader(ShaderType type, GLint coreVersion)
	: m_type(type), m_coreVersion(coreVersion), m_loadFromPaths(false) {
	m_id = glCreateShader((GLenum)m_type);
}

Shader::~Shader() {
	glDeleteShader(m_id);
}

void Shader::setCoreVersion(GLuint coreVersion) {
	m_coreVersion = coreVersion;
}

void Shader::setDefines(const std::vector<std::string>& defines) {
	m_defines = defines;
}

void Shader::setSourceFile(const std::string & path) {
	m_loadFromPaths = true;
	m_paths.clear();
	m_paths.push_back(path);
}

void Shader::setSourceFiles(const std::vector<std::string>& paths) {
	m_loadFromPaths = true;
	m_paths = paths;

}

void Shader::setSource(const std::string & source) {
	m_loadFromPaths = false;
	m_source = source;
}

void Shader::setSources(const std::vector<std::string>& sources) {
	std::string finalSource;
	for (auto &source : sources) {
		finalSource += source;
	}
	m_source = finalSource;
}

GLuint Shader::compile() const {
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
	glShaderSource(m_id, 1, &shaderCode, NULL);
	glCompileShader(m_id);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(m_id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	return m_id;
}

ShaderType Shader::getType() const {
	return m_type;
}

}