#ifndef MESH_H
#define MESH_H

#include <vector>

#include "Material.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "openGL.h"

typedef struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
} Vertex;

class Mesh {
public:
	Mesh();
	~Mesh();

	void draw();
	void setupMesh();
public:
    /*  Mesh Data  */
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
	std::string name;
	std::string shading;

	//Material
	Material material;
private:
    //Render
    GLuint VAO, VBO, EBO;
};

#endif // MESH_H
