#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <unordered_map>

#include "Camera.h"
#include "SceneObject.h"
#include "Light.h"
#include "Shader.h"
#include "Texture.h"

#include "../utils/Arrow.h"

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
	void computeShaders(const SceneObjectPtr &node);

	void renderNode(const SceneObjectPtr &node, const MeshShaderPtr &shader, const ShaderConfiguration &configuration, glm::mat4 accTransform);

	std::unordered_map<std::string, MeshShaderPtr> shaders;

	Arrow m_arrow;
	ShaderProgramGL m_arrowShader;
};

#endif // !SCENE_H
