#include "mesh.h"

#include <string>
#include <sstream>
#include <OpenGL.h>

#include "glassert.h"

using namespace std;

Mesh::Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures)
    : vertices(vertices), indices(indices), textures(textures), VAO(0), VBO(0), EBO(0)
{
    setupMesh();
}

Mesh::~Mesh()
{
//    glDeleteVertexArrays(1, &VAO);
//    glDeleteBuffers(1, &VBO);
}

void Mesh::draw(Shader shader, const glm::mat4 &model)
{
	shader.setUniform("model", model);
	shader.setUniform("normalModel", glm::mat3(glm::transpose(glm::inverse(model))));
	shader.setUniform("material.shininess", 16.f);

    GLuint diffuseNr = 1;
    GLuint specularNr = 1;
    for(GLuint i = 0; i < textures.size(); i++)
    {
        glAssert(glActiveTexture(GL_TEXTURE0 + i)); // Activate proper texture unit before binding

        // Retrieve texture number (the N in diffuse_textureN)
        stringstream ss;
        string number;
        string name = textures[i].type;
        if(name == "diffuse")
            ss << diffuseNr++; // Transfer GLuint to stream
        else if(name == "specular")
            ss << specularNr++; // Transfer GLuint to stream
        number = ss.str();

        string finalName = "material." + name + number;

		shader.setUniform(finalName, i);
		textures[i].bind();
    }
    glAssert(glActiveTexture(GL_TEXTURE0));

    // Draw mesh
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

    glAssert(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
    glAssert(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
                 &indices[0], GL_STATIC_DRAW));

    glAssert(glBindVertexArray(0));
}
