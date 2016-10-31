#include "SceneObject.h"

SceneObject *SceneObject::createChild()
{
	childs.push_back(SceneObjectPtr(new SceneObject));
	return childs.back().get();
}
