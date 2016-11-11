#include "Mesh.h"

#include <string>
#include <sstream>

Mesh::Mesh() {}

void Mesh::addVBO(gl::BufferPtr VBO) {
	m_vertexArray.bindVertexBuffer(VBO);
}

void Mesh::addVBO(std::initializer_list<gl::BufferPtr> VBO) {
	m_vertexArray.bindVertexBuffer(VBO);
}

void Mesh::addIBO(gl::BufferPtr IBO, GLuint indicesCount) {
	indiceCount = indicesCount;
	m_vertexArray.bindVertexBuffer(IBO);
}

void Mesh::bindVAO() const {
	m_vertexArray.bind();
}

void Mesh::unbindVAO() const {
	m_vertexArray.unbind();
}

void ModelMesh::draw() const {
	bindVAO();
	glDrawElements(GL_TRIANGLES, indiceCount, GL_UNSIGNED_INT, 0);
	unbindVAO();
}
