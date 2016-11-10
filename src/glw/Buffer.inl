#include "Buffer.h"

namespace gl {

template <typename DataType>
void Buffer::setData(const std::vector<DataType> &data, BufferUsage usage) {
	glBindBuffer(m_target, m_id);
	glBufferData(m_target, data.size() * sizeof(DataType),
				 &data[0], usage);
}

template<typename DataType>
void Buffer::setData(const DataType * data, GLsizeiptr size, BufferUsage usage) {
	glBindBuffer(m_target, m_id);
	glBufferData(m_target, size, data, usage);
}

template<typename DataType>
void Buffer::setSubData(const std::vector<DataType> &data, GLintptr offset) {
	glBindBuffer(m_target, m_id);
	glBufferSubData(m_target, offset, data.size() * sizeof(DataType), &data[0]);
}

template<typename DataType>
void Buffer::setSubData(const DataType *data, GLintptr offset, GLsizeiptr size) {
	glBindBuffer(m_target, m_id);
	glBufferSubData(m_target, offset, size, data);
}

}