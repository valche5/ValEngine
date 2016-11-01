#ifndef MESH_H
#define MESH_H

#include <vector>

#include "texture.h"
#include "Material.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Mesh {
public:
	Mesh();
	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);
	~Mesh();
	void draw();
	void setupMesh();
public:
    /*  Mesh Data  */
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
	std::string name;
	std::string shading;

	//Material
	Material material;
private:
    //Render
    GLuint VAO, VBO, EBO;
};

#endif // MESH_H
