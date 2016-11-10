#include "SceneObject.h"

#include <glm/gtc/matrix_transform.hpp>

#include <utils/glmstreams.h>
#include <glw/Texture.h>

SceneObject *SceneObject::createChild()
{
	childs.push_back(SceneObjectPtr(new SceneObject));
	return childs.back().get();
}

void SceneObject::translate(glm::vec3 translation) {
	transform = glm::translate(transform, translation);
}

void SceneObject::scale(glm::vec3 scaling) {
	transform = glm::scale(transform, scaling);
}

std::string SceneObject::toString() {
	std::stringstream result;
	std::string space;

	SceneObject *p = parent;
	while (p != nullptr) {
		space += "   ";
		p = p->parent;
	}
	result << space << "-Object " << name << std::endl;
	for (auto &mesh : meshes) {
		result << space << " -Mesh " << mesh->name << ", " << mesh->vertexCount << " vertices, " << mesh->indiceCount << " indices" << std::endl;
		Material &mat = mesh->material;
		result << space << "  -Material " << mat.name << " shininess(" << mat.getProperty<float>(Shininess) << ") "
			<< "ka" << mat.getProperty<glm::vec3>(Ka) << " "
			<< "kd" << mat.getProperty<glm::vec3>(Kd) << " "
			<< "ks" << mat.getProperty<glm::vec3>(Ks) << " "
			<< std::endl;
		for (auto &tex : mat.getTextures())
			result << space << "  -Texture (" << textureTypeString.at(tex.first) + ") " << tex.second->name << std::endl;
	}

	for (auto &child : childs) {
		result << child->toString();
	}

	return result.str();
}
