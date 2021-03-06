#ifndef VERTEXARRAYGL_H
#define VERTEXARRAYGL_H

#include <unordered_map>
#include <vector>
#include <initializer_list>

#include <glw/GLObject.h>
#include <glw/Types.h>
#include <glw/Buffer.h>

namespace gl {

class VertexArray : public GLObject {
public:
	VertexArray();
	~VertexArray();

	void bindVertexBuffer(BufferPtr buffer);
	void bindVertexBuffer(std::initializer_list<BufferPtr> buffers);

	void bind() const;
	void unbind() const;
private:
	std::vector<BufferPtr> m_buffers;
};

}

#endif // VAO_H
