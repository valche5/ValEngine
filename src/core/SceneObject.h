#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <iostream>
#include <sstream>
#include <vector>
#include <memory>

#include <glm/glm.hpp>
#include <glm/matrix.hpp>

#include "mesh.h"
#include "Material.h"

class SceneObject;
class Scene;

typedef std::unique_ptr<SceneObject> SceneObjectPtr;

class SceneObject {
public:
	SceneObject *createChild();

	void translate(glm::vec3 translation);
	void scale(glm::vec3 scaling);

	std::vector<Mesh> meshes;
	Material material;

	std::vector<SceneObjectPtr> childs;
	SceneObject *parent;

	glm::mat4 transform;
	std::string name;

	Scene *scene;
public:
	std::string toString() {
		std::stringstream result;
		std::string space;

		SceneObject *p = parent;
		while (p != nullptr) {
			space += "   ";
			p = p->parent;
		}
		result << space << "-Object " << name << std::endl;
		for (auto &mesh : meshes) {
			result << space << " -Mesh " << mesh.name << ", " << mesh.vertices.size() << " vertices, " << mesh.indices.size() << " indices" << std::endl;
			Material &mat = mesh.material;
			result << space << "  -Material " << mat.name << " shininess(" << mat.shininess << ") "
				<< "ka(" << mat.ka.r << "," << mat.ka.g << "," << mat.ka.b << ") "
				<< "kd(" << mat.kd.r << "," << mat.kd.g << "," << mat.kd.b << ") "
				<< "ks(" << mat.ks.r << "," << mat.ks.g << "," << mat.ks.b << ") "
				<< std::endl;
			for (auto &tex : mat.textures) 
				result << space << "  -Texture (" << mat.textureTypeString(tex.first) + ") " << tex.second << std::endl;
		}

		for (auto &child : childs) {
			result << child->toString();
		}

		return result.str();
	}
};

#endif // SCENEOBJECT_H
