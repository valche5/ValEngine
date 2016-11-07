#ifndef MESH_H
#define MESH_H

#include <memory>
#include <vector>

#include <utils/BoundingBox.h>
#include <core/Material.h>
#include <core/openGL.h>
#include <glw/VertexArray.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

typedef struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
} Vertex;

class Mesh {
public:
	void setupMesh();
	void draw();
public:
    // Mesh Data
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
	std::string name;
	std::string shading;

	//Material
	Material material;

	//Bounding Box
	AABB bBox;
private:
    //Render
	gl::VertexArray m_vertexArray;
};


#endif // MESH_H
