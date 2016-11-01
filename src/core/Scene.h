#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <unordered_map>

#include "camera.h"
#include "SceneObject.h"
#include "Light.h"
#include "PhongShader.h"

class Scene;

typedef std::unique_ptr<Scene> ScenePtr;

class Scene {
public:
	Scene();
	void init();
	void render();

	SceneObject *getRootObject();
	Camera *getCamera();
	void reloadShaders();

	std::string toString() {
		return rootObject->toString();
	}


	
	SceneObjectPtr rootObject;
	CameraPtr m_camera;

	std::unordered_map<std::string, Texture> textures;
	std::vector<PointLight> pointLights;
	std::vector<DirLight> dirLights;
	std::vector<SpotLight> spotLights;

	
private:
	void computeShaders();
	void computeShaders(SceneObjectPtr &node);

	void renderNode(SceneObjectPtr &node, const PhongShader &shader, const ShaderConfiguration &configuration, glm::mat4 accTransform);

	std::unordered_map<std::string, PhongShader> shaders;
};

#endif // !SCENE_H
