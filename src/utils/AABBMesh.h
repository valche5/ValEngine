#ifndef AABBMESH_H
#define AABBMESH_H

#include <core/Mesh.h>

class AABBMesh : public Mesh {
public:
	AABBMesh() {
		GLfloat vertices[] = {
			-0.5f, -0.5f, 0.5f,
			0.5f, -0.5f, 0.5f,
			0.5f, 0.5f, 0.5f,
			-0.5f, 0.5f, 0.5f,
			-0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, 0.5f, -0.5f,
			-0.5f, 0.5f, -0.5f,
		};
		
		GLuint indices[] = {
			0, 1,
			1, 2,
			2, 3,
			3, 0,
			4, 5,
			5, 6,
			6, 7,
			7, 4,
			0, 4,
			1, 5,
			2, 6,
			3, 7
		};

		VBOFactory vbof;
		vbof.setVertexCount(8);
		vbof.setData(VertexPosition, vertices);
		addVBO(vbof.getBlockVBO());

		IBOFactory ibof;
		ibof.setData(indices, 24);
		addIBO(ibof.getIBO(), 24);

		shaderConf.shadingType = "fill";
		material.addProperty(FillColor, glm::vec3(0.4, 1, 0.4));
	}
	virtual void draw() const {
		bindVAO();
		glDrawElements(GL_LINES, indiceCount, GL_UNSIGNED_INT, 0);
		unbindVAO();
	};
};

#endif // AABBMESH_H
