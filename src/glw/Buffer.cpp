#include "Buffer.h"

namespace gl {

Buffer::Buffer(BufferTarget target)
	: m_target(target), m_stride(0) {
	glGenBuffers(1, &m_id);
}

Buffer::~Buffer() {
	glDeleteBuffers(1, &m_id);
}

template <typename DataType>
void setData(const std::vector<DataType> &data, BufferUsage usage = StaticDraw) {
	m_stride = sizeof(DataType);
	glBindBuffer(m_target, m_id);
	glBufferData(m_target, data.size() * sizeof(DataType),
				 &data[0], usage);
}

void Buffer::setAttributes(const std::vector<Attribute> &attributes) {
	m_attributes = attributes;
}

const std::vector<Attribute> &Buffer::getAttributes() const {
	return m_attributes;
}

GLsizei Buffer::getStride() const {
	return m_stride;
}

BufferTarget Buffer::target() const {
	return m_target;
}

}