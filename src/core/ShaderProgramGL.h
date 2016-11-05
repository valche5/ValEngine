#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "OpenGL.h"
#include "../utils/File.h"

enum class ShaderType : GLenum {
	Vertex = GL_VERTEX_SHADER,
	Fragment = GL_FRAGMENT_SHADER
};

class ShaderGL {
public:
	ShaderGL(ShaderType type, GLint coreVersion = 330);
	~ShaderGL();

	void setCoreVersion(GLuint coreVersion);
	void setDefines(const std::vector<std::string> &m_defines);

	void setSourceFile(const std::string &path);
	void setSourceFiles(const std::vector<std::string> &paths);

	void setSource(const std::string &source);
	void setSources(const std::vector<std::string> &sources);

	GLuint compile() const;

	ShaderType getType() const;

	GLuint glId();

	operator GLuint() const { return m_shader; }
private:
	GLuint m_coreVersion;
	std::vector<std::string> m_defines;
	std::string m_source;
	std::vector <std::string> m_paths;
	ShaderType m_type;
	GLuint m_shader;

	bool m_loadFromPaths;
};

typedef std::shared_ptr<ShaderGL> ShaderGLPtr;

class ShaderProgramGL {
public:
    ShaderProgramGL();
    ~ShaderProgramGL();

	void attach(const ShaderGLPtr &shader);
	void compile() const;

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
	GLuint m_program;

	std::unordered_map<ShaderType, ShaderGLPtr> m_shaders;
};

#endif // SHADER

