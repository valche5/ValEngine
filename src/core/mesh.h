#ifndef MESH_H
#define MESH_H

#include <core/Material.h>
#include <core/openGL.h>
#include <glw/VertexArray.h>
#include <utils/BoundingBox.h>

class Mesh {
public:
	Mesh();

	void addVBO(gl::BufferPtr VBO);
	void addIBO(gl::BufferPtr IBO, GLuint indicesCount);

	void draw();
public:
	//Properties
	std::string name;
	std::string shading;
	//Counts
	GLuint vertexCount = 0;
	GLuint indiceCount = 0;

	//Material
	Material material;

	//Bounding Box
	AABB bBox;
private:
    //Render
	gl::VertexArray m_vertexArray;
	
};


#endif // MESH_H
