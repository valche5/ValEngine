#include "SceneObject.h"

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
