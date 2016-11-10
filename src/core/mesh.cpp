#include "Mesh.h"

#include <string>
#include <sstream>

Mesh::Mesh() {}

void Mesh::addVBO(gl::BufferPtr VBO) {
	m_vertexArray.bindVertexBuffer(VBO);
}

void Mesh::addIBO(gl::BufferPtr IBO, GLuint indicesCount) {
	indiceCount = indicesCount;
	m_vertexArray.bindVertexBuffer(IBO);
}


void Mesh::draw()
{
	m_vertexArray.bind();
	glDrawElements(GL_TRIANGLES, indiceCount, GL_UNSIGNED_INT, 0);
	m_vertexArray.unbind();
}

