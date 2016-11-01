#include "mesh.h"

#include <string>
#include <sstream>
#include "OpenGL.h"

#include "glassert.h"

using namespace std;

Mesh::Mesh()
{
}

Mesh::Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures)
    : vertices(vertices), indices(indices), textures(textures), VAO(0), VBO(0), EBO(0)
{
    setupMesh();
}

Mesh::~Mesh()
{
    //glDeleteVertexArrays(1, &VAO);
    //glDeleteBuffers(1, &VBO);
}

void Mesh::draw()
{
    glAssert(glBindVertexArray(VAO));
    glAssert(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0));
    glAssert(glBindVertexArray(0));
}

void Mesh::setupMesh()
{
    glAssert(glGenVertexArrays(1, &VAO));
    glAssert(glGenBuffers(1, &VBO));
    glAssert(glGenBuffers(1, &EBO));

    glAssert(glBindVertexArray(VAO));

    glAssert(glBindBuffer(GL_ARRAY_BUFFER, VBO));

    glAssert(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                 &vertices[0], GL_STATIC_DRAW));

    // Vertex Positions
    glAssert(glEnableVertexAttribArray(0));
    glAssert(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                         (GLvoid*)0));

    // Vertex Normals
    glAssert(glEnableVertexAttribArray(1));
    glAssert(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                         (GLvoid*)offsetof(Vertex, Normal)));

    // Vertex Texture Coords
    glAssert(glEnableVertexAttribArray(2));
    glAssert(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                         (GLvoid*)offsetof(Vertex, TexCoords)));

	//Indices
    glAssert(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
    glAssert(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
                 &indices[0], GL_STATIC_DRAW));

    glAssert(glBindVertexArray(0));
}
