#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <iostream>
#include <sstream>
#include <vector>
#include <memory>

#include <glm/glm.hpp>
#include <glm/matrix.hpp>

#include <core/Mesh.h>
#include <core/Types.h>

class SceneObject;
class Scene;

typedef std::unique_ptr<SceneObject> SceneObjectPtr;

class SceneObject {
public:
	SceneObjectPtr &createChild();

	void translate(glm::vec3 translation);
	void scale(glm::vec3 scaling);
	std::string toString();

public:
	//Tree
	std::vector<SceneObjectPtr> childs;
	SceneObject *parent;

	//Meshes
	std::vector<MeshPtr> meshes;
	Material material;

	//Parameters
	glm::mat4 transform;
	std::string name;
	AABB bBox;

	Scene *scene;
};

#endif // SCENEOBJECT_H
