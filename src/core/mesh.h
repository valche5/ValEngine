#ifndef MESH_H
#define MESH_H

#include <initializer_list>

#include <core/openGL.h>
#include <core/Material.h>
#include <core/ShaderConfiguration.h>
#include <core/VBOFactory.h>
#include <core/IBOFactory.h>
#include <glw/VertexArray.h>
#include <utils/BoundingBox.h>

class Mesh {
public:
	Mesh();

	void addVBO(gl::BufferPtr VBO);
	void addVBO(std::initializer_list<gl::BufferPtr> VBO);
	void addIBO(gl::BufferPtr IBO, GLuint indicesCount);

	virtual void draw() const = 0;
public:
	//Properties
	std::string name;
	ShaderConfiguration shaderConf;

	//Counts
	GLuint vertexCount = 0;
	GLuint indiceCount = 0;

	//Material
	Material material;

	//Bounding Box
	AABB bBox;
protected:
	void bindVAO() const;
	void unbindVAO() const;
    //Render
	gl::VertexArray m_vertexArray;
};

class ModelMesh : public Mesh {
public:
	virtual void draw() const;
};


#endif // MESH_H
