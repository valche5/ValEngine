#ifndef GLOBJECT_H
#define GLOBJECT_H

#include <core/openGL.h>

namespace gl {

class GLObject {
public:
	GLObject() {};
	GLObject(const GLObject &) = delete;
	GLObject &operator=(const GLObject &) = delete;
	GLObject(GLObject &&) = default;
	GLObject &operator=(GLObject &&) = default;


	GLuint getId() {
		return m_id;
	}
	operator GLuint() {
		return m_id;
	}
protected:
	GLuint m_id;
};

}

#endif // GLOBJECT_H
