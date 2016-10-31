#include "Scene.h"

#include <memory>

Scene::Scene() : rootObject(new SceneObject) {
	rootObject->parent = nullptr;
}

SceneObject *Scene::getRootObject()
{
	return rootObject.get();
}
