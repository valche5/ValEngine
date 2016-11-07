#include "Mesh.h"

#include <string>
#include <sstream>

void Mesh::setupMesh() {
	gl::BufferPtr VBO(new gl::Buffer(gl::BufferTarget::ArrayBuffer));
	VBO->setData(vertices);
	VBO->setAttributes({
		{ 0, 3, GL_FLOAT, 0 },
		{ 1, 3, GL_FLOAT, offsetof(Vertex, Normal) },
		{ 2, 3, GL_FLOAT, offsetof(Vertex, TexCoords) },
	});

	gl::BufferPtr EBO(new gl::Buffer(gl::BufferTarget::ElementBuffer));
	EBO->setData(indices);

	m_vertexArray.bindVertexBuffers({ VBO, EBO });
}

void Mesh::draw()
{
	m_vertexArray.bind();
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	m_vertexArray.unbind();
}

