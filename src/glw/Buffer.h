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
	Attribute(GLuint index = 0, GLuint size = 0, GLenum type = GL_FLOAT, size_t offset = 0, GLboolean normalized = GL_FALSE)
		: index(index), size(size), type(type), normalized(normalized), offset(offset) {};
	GLuint index;
	GLuint size;
	GLenum type;
	GLboolean normalized;
	size_t offset;
};

class Buffer : public GLObject {
public:
	Buffer(BufferTarget target);
	~Buffer();

	template <typename DataType>
	void setData(const std::vector<DataType> &data, BufferUsage usage = StaticDraw);
	void setAttributes(const std::vector<Attribute> &attributes);

	const std::vector<Attribute> &getAttributes() const;
	GLsizei getStride() const;
	BufferTarget target() const;
private:
	BufferTarget m_target;
	GLsizei m_stride;
	std::vector<Attribute> m_attributes;
};

}

#include "Buffer.inl"

#endif // BUFFERGL_H
