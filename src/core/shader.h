#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <vector>
#include "OpenGL.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
    Shader();
    ~Shader();

    void load(const char *vertexPath, const char *fragmentPath, const std::vector<std::string> &defines = std::vector<std::string>());
    void reload();
    void use();

	void setUniform(const std::string &loc, glm::vec3 u3f);
	void setUniform(const std::string &loc, glm::mat3 um3fv);
	void setUniform(const std::string &loc, glm::mat4 um4fv);
	void setUniform(const std::string &loc, GLfloat u1f);
	void setUniform(const std::string &loc, GLuint u1i);

    operator GLuint() { return program; }

    GLuint program;

private:
	void load();

    const char *m_vertexPath;
    const char *m_fragmentPath;

	std::vector<std::string> m_defines;
};

#endif // SHADER

