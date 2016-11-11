#include "VertexArray.h"

namespace gl {

VertexArray::VertexArray() {
	glGenVertexArrays(1, &m_id);
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &m_id);
}

void VertexArray::bindVertexBuffer(BufferPtr buffer) {
	m_buffers.push_back(buffer);
	glBindVertexArray(m_id);
	glBindBuffer(buffer->target(), *buffer);
	//Attributes
	for (auto &attr : buffer->getAttributes()) {
		glEnableVertexAttribArray(attr.index);
		glVertexAttribPointer(attr.index, attr.size, attr.type, attr.normalized, attr.stride,
			(GLvoid*)attr.offset);
	}
	glBindVertexArray(0);
}

void VertexArray::bindVertexBuffer(std::initializer_list<BufferPtr> buffers) {
	glBindVertexArray(m_id);
	for (auto &buffer : buffers) {
		m_buffers.push_back(buffer);
		glBindBuffer(buffer->target(), *buffer);
		//Attributes
		for (auto &attr : buffer->getAttributes()) {
			glEnableVertexAttribArray(attr.index);
			glVertexAttribPointer(attr.index, attr.size, attr.type, attr.normalized, attr.stride,
				(GLvoid*)attr.offset);
		}
	}
	glBindVertexArray(0);
}

void VertexArray::bind() const { 
	glBindVertexArray(m_id);
}

void VertexArray::unbind() const {
	glBindVertexArray(0);
}

}