#ifndef SHADERGL_H
#define SHADERGL_H

#include <vector>

#include <glw/GLObject.h>

namespace gl {

enum class ShaderType : GLenum {
	Vertex = GL_VERTEX_SHADER,
	Fragment = GL_FRAGMENT_SHADER
};

class Shader : public GLObject {
public:
	Shader(ShaderType type, GLint coreVersion = 330);
	~Shader();

	void setCoreVersion(GLuint coreVersion);
	void setDefines(const std::vector<std::string> &m_defines);

	void setSourceFile(const std::string &path);
	void setSourceFiles(const std::vector<std::string> &paths);

	void setSource(const std::string &source);
	void setSources(const std::vector<std::string> &sources);

	GLuint compile() const;

	ShaderType getType() const;
private:
	GLuint m_coreVersion;
	std::vector<std::string> m_defines;
	std::string m_source;
	std::vector <std::string> m_paths;
	ShaderType m_type;

	bool m_loadFromPaths;
};

}



#endif // SHADER_H
