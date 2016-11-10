#include "Buffer.h"

namespace gl {

Buffer::Buffer(BufferTarget target)
	: m_target(target) {
	glGenBuffers(1, &m_id);
}

Buffer::~Buffer() {
	glDeleteBuffers(1, &m_id);
}

void Buffer::bind() {
	glBindBuffer(m_target, m_id);
}

void Buffer::allocate(GLsizei size, BufferUsage usage) {
	glBindBuffer(m_target, m_id);
	glBufferData(m_target, size, NULL, usage);
}

void Buffer::addAttribute(const Attribute & attribute) {
	m_attributes.push_back(attribute);
}

void Buffer::setAttributes(const std::vector<Attribute> &attributes) {
	m_attributes = attributes;
}

const std::vector<Attribute> &Buffer::getAttributes() const {
	return m_attributes;
}

BufferTarget Buffer::target() const {
	return m_target;
}

}