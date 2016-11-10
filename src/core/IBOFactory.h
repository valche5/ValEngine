#ifndef IBOFACTORY_H
#define IBOFACTORY_H

#include <glw/Buffer.h>
#include <glw/Types.h>

class IBOFactory {
public:
	void setData(const GLuint *data, GLuint indicesCount) {
		m_indices = data;
		m_indicesCount = indicesCount;
	};
	gl::BufferPtr getIBO() const {
		gl::BufferPtr IBO(new gl::Buffer(gl::ElementBuffer));
		
		glBindBuffer(gl::ElementBuffer, IBO->getId());
		glBufferData(gl::ElementBuffer, m_indicesCount * sizeof(GLuint), m_indices, gl::StaticDraw);

		return IBO;
	};
private:
	const GLuint *m_indices;
	GLuint m_indicesCount;
};

#endif // IBOFACTORY_H
