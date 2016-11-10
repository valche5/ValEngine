#ifndef BUFFERGL_H
#define BUFFERGL_H

#include <vector>

#include <glw/GLObject.h>

namespace gl {

enum BufferTarget : GLenum {
	ArrayBuffer = GL_ARRAY_BUFFER,
	ElementBuffer = GL_ELEMENT_ARRAY_BUFFER,
	AtomicCounterBuffer = GL_ATOMIC_COUNTER_BUFFER,
	CopyReadBuffer = GL_COPY_READ_BUFFER,
	CopyWriteBuffer = GL_COPY_WRITE_BUFFER,
	DispatchIndirectBuffer = GL_DISPATCH_INDIRECT_BUFFER,
	DrawIndirectBuffer = GL_DRAW_INDIRECT_BUFFER,
	PixelPackBuffer = GL_PIXEL_PACK_BUFFER,
	PixelUnpackBuffer = GL_PIXEL_UNPACK_BUFFER,
	QueryBuffer = GL_QUERY_BUFFER,
	ShaderStorageBuffer = GL_SHADER_STORAGE_BUFFER,
	TextureBuffer = GL_TEXTURE_BUFFER,
	TransformFeedbackBuffer = GL_TRANSFORM_FEEDBACK_BUFFER,
	UniformBuffer = GL_UNIFORM_BUFFER
};

enum BufferUsage : GLenum {
	StreamDraw = GL_STREAM_DRAW,
	StramRead = GL_STREAM_READ,
	StreamCopy = GL_STREAM_COPY,
	StaticDraw = GL_STATIC_DRAW,
	StaticRead = GL_STATIC_READ,
	StaticCopy = GL_STATIC_COPY,
	DynamicDraw = GL_DYNAMIC_DRAW,
	DynamicRead = GL_DYNAMIC_READ,
	DynamicCopy = GL_DYNAMIC_COPY
};

struct Attribute {
	//index, size, type, stride, offset, normalizeds
	Attribute(GLuint index = 0, GLuint size = 0, GLenum type = GL_FLOAT, GLsizei stride = 0, size_t offset = 0, GLboolean normalized = GL_FALSE)
		: index(index), size(size), type(type), stride(stride), normalized(normalized), offset(offset) {};
	//Specifies the index of the generic vertex attribute to be modified.
	GLuint index;
	//Specifies the number of components per generic vertex attribute. Must be 1, 2, 3, 4. Additionally, the symbolic constant GL_BGRA is accepted by glVertexAttribPointer. The initial value is 4.
	GLuint size;
	//Specifies the data type of each component in the array
	GLenum type;
	//Specifies the byte offset between consecutive generic vertex attributes.
	GLsizei stride;
	//if normalized is set to GL_TRUE, it indicates that values stored in an integer format are to be mapped to the range [-1,1] (for signed values) or [0,1] (for unsigned values)
	GLboolean normalized;
	//Specifies a offset of the first component of the first generic vertex attribute in the array in the data store of the buffer currently bound to the GL_ARRAY_BUFFER target.
	size_t offset;
};

class Buffer : public GLObject {
public:
	Buffer(BufferTarget target);
	~Buffer();

	void bind();

	template <typename DataType>
	void setData(const std::vector<DataType> &data, BufferUsage usage = StaticDraw);
	template <typename DataType>
	void setData(const DataType *data, GLsizeiptr size, BufferUsage usage = StaticDraw);

	void allocate(GLsizei size, BufferUsage usage = StaticDraw);
	template <typename DataType>
	void setSubData(const std::vector<DataType> &data, GLintptr offset);
	template <typename DataType>
	void setSubData(const DataType *data, GLintptr offset, GLsizeiptr size);

	void addAttribute(const Attribute &attribute);
	void setAttributes(const std::vector<Attribute> &attributes);

	const std::vector<Attribute> &getAttributes() const;
	BufferTarget target() const;
private:
	BufferTarget m_target;
	std::vector<Attribute> m_attributes;
};

}

#include "Buffer.inl"

#endif // BUFFERGL_H
