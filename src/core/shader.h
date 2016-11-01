#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "OpenGL.h"

struct uPointLight {
	GLuint position;
	GLuint constant;
	GLuint linear;
	GLuint quadratic;
	GLuint ambient;
	GLuint diffuse;
	GLuint specular;
};

struct uDirLight {
	GLuint direction;
	GLuint ambient;
	GLuint diffuse;
	GLuint specular;
};

struct uSpotLight {
	GLuint position;
	GLuint direction;
	GLuint cutOff;
	GLuint outerCutOff;
	GLuint constant;
	GLuint linear;
	GLuint quadratic;
	GLuint ambient;
	GLuint diffuse;
	GLuint specular;
};

class Shader
{
public:
    Shader();
    ~Shader();

	void load(const std::string &vertexPath, const std::vector<std::string> &fragmentPaths, const std::vector<std::string> &defines = std::vector<std::string>());
    void reload();
    void use() const;

	template<typename T>
	void setUniform(const std::string &loc, T value) const {
		setUniform(glGetUniformLocation(m_program, loc.c_str()), value);
	};

	void setUniform(GLuint loc, glm::vec3 u3f) const;
	void setUniform(GLuint loc, glm::mat3 um3fv) const;
	void setUniform(GLuint loc, glm::mat4 um4fv) const;
	void setUniform(GLuint loc, GLfloat u1f) const;
	void setUniform(GLuint loc, GLuint u1i) const;

    operator GLuint() { return m_program; }

protected:
	void load();

protected:
	GLuint m_program;

	std::string directory = "shaders/";
	std::string m_vertexPath;
	std::vector<std::string> m_fragmentPaths;

	std::vector<std::string> m_defines;
};

#endif // SHADER

