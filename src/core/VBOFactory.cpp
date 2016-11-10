#include "VBOFactory.h"

void VBOFactory::setVertexCount(GLuint vertexCount) {
	m_vertexCount = vertexCount;
}

void VBOFactory::setData(VertexAttribName attribName, const GLfloat * data) {
	assert(m_vertexCount > 0 && "Vous devez spécifier un nombre de Vertex avant de pouvoir ajouter des données");
	m_attribData[attribName] = data;
	m_attribDataSize[attribName] = m_vertexCount * attributeGL.at(attribName).size * sizeof(GLfloat);
	m_attribNamesFlag |= attribName;
	m_attribNames.insert(attribName);
}

gl::BufferPtr VBOFactory::getInterleavedVBO() const {
	checkValidity();
	return gl::BufferPtr(nullptr);
}

gl::BufferPtr VBOFactory::getBlockVBO() const {
	checkValidity();
	//Generate VBO
	gl::BufferPtr VBO(new gl::Buffer(gl::ArrayBuffer));

	//Calc VBO total size
	GLsizeiptr bufferSize = 0;
	for (auto sizepair : m_attribDataSize)
		bufferSize += sizepair.second;

	//Allocate VBO
	glBindBuffer(gl::ArrayBuffer, VBO->getId());
	glBufferData(gl::ArrayBuffer, bufferSize, 0, gl::StaticDraw);
	GLintptr offset = 0;
	for (auto attribName : m_attribNames) {
		//Fill VBO
		GLsizeiptr size = m_attribDataSize.at(attribName);
		const GLfloat *data = m_attribData.at(attribName);
		glBufferSubData(gl::ArrayBuffer, offset, size, data);

		//Set glAttrib
		gl::Attribute attr = attributeGL.at(attribName);
		attr.offset = offset;
		attr.stride = attr.size * sizeof(GLfloat);
		VBO->addAttribute(attr);

		offset += size;
	}
	return VBO;
}

std::vector<gl::BufferPtr> VBOFactory::getPerAttribVBO() const {
	checkValidity();
	return std::vector<gl::BufferPtr>();
}

void VBOFactory::checkValidity() const {
	if (m_vertexCount > 0) {
		assert((m_attribNamesFlag & VertexPosition)
			   && "Vous devez au moins spécifier la position des Vertex pour générer le VBO");
	}
}
