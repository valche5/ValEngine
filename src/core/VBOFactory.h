#ifndef VBOFACTORY_H
#define VBOFACTORY_H

#include <cassert>
#include <unordered_map>
#include <set>

#include <glw/Buffer.h>
#include <glw/Types.h>


enum VertexAttribName : unsigned char {
	VertexPosition = 0x01,
	VertexNormal = 0x02,
	VertexTexCoord = 0x04,
	VertexTangent = 0x08,
	VertexBitangent = 0x10
};

typedef unsigned char AttribFlag;

static const std::unordered_map<VertexAttribName, gl::Attribute> attributeGL{
	{ VertexPosition,{ 0, 3, GL_FLOAT } },
	{ VertexNormal,{ 1, 3, GL_FLOAT } },
	{ VertexTexCoord,{ 2, 2, GL_FLOAT } },
	{ VertexTangent,{ 3, 3, GL_FLOAT } },
	{ VertexBitangent,{ 4, 3, GL_FLOAT } }
};

class VBOFactory {
public:
	void setVertexCount(GLuint vertexCount);
	void setData(VertexAttribName attribName, const GLfloat *data);

	gl::BufferPtr getInterleavedVBO() const;
	gl::BufferPtr getBlockVBO() const;
	std::vector<gl::BufferPtr> getPerAttribVBO() const;
private:
	void checkValidity() const;
private:
	GLuint m_vertexCount = 0;

	std::unordered_map<VertexAttribName, const GLfloat *> m_attribData;
	std::unordered_map<VertexAttribName, GLsizeiptr> m_attribDataSize;
	std::set<VertexAttribName> m_attribNames;

	AttribFlag m_attribNamesFlag;

};

#endif // VBOFACTORY_H
