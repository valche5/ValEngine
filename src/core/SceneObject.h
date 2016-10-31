#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>

#include "mesh.h"

class SceneObject;

typedef std::unique_ptr<SceneObject> SceneObjectPtr;

class SceneObject {
public:
	SceneObject *createChild();

	std::vector<Mesh> meshes;

	std::vector<SceneObjectPtr> childs;
	SceneObject *parent;

	glm::mat4 transform;
	std::string name;

	std::string toString() {
		std::string result;
		std::string space;

		SceneObject *p = parent;
		while (p != nullptr) {
			space += "   ";
			p = p->parent;
		}
		result = space + "-Object " + name + "\n";
		for (auto &mesh : meshes) {
			result += space + " -Mesh " + mesh.name + ", " + std::to_string(mesh.vertices.size()) + " vertices, " + std::to_string(mesh.indices.size()) + " indices\n";
		}

		for (auto &child : childs) {
			result += child->toString();
		}

		return result;
	}
};

#endif // SCENEOBJECT_H
