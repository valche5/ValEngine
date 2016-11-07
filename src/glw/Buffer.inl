
namespace gl {

template <typename DataType>
void Buffer::setData(const std::vector<DataType> &data, BufferUsage usage) {
	m_stride = sizeof(DataType);
	glBindBuffer(m_target, m_id);
	glBufferData(m_target, data.size() * sizeof(DataType),
				 &data[0], usage);
};

}