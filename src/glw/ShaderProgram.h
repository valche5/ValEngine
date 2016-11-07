#ifndef SHADERPROGRAMGL_H
#define SHADERPROGRAMGL_H

#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <utils/File.h>
#include <glw/Types.h>
#include <glw/GLObject.h>
#include <glw/Shader.h>

namespace gl {

class ShaderProgram : public GLObject {
public:
	ShaderProgram();
	~ShaderProgram();

	void attach(const ShaderPtr &shader);
	void compile() const;

	void use() const;

	template<typename T>
	void setUniform(const std::string &loc, T value) const {
		setUniform(glGetUniformLocation(m_id, loc.c_str()), value);
	};

	void setUniform(GLuint loc, glm::vec3 u3f) const;
	void setUniform(GLuint loc, glm::mat3 um3fv) const;
	void setUniform(GLuint loc, glm::mat4 um4fv) const;
	void setUniform(GLuint loc, GLfloat u1f) const;
	void setUniform(GLuint loc, GLuint u1i) const;

protected:
	std::unordered_map<ShaderType, ShaderPtr> m_shaders;
};

}


#endif // SHADERPROGRAMGL

