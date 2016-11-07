#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <unordered_map>

#include <core/Camera.h>
#include <core/SceneObject.h>
#include <core/Light.h>
#include <core/MeshShader.h>
#include <core/Types.h>
#include <glw/Texture.h>
#include <glw/ShaderProgram.h>
#include <utils/Arrow.h>

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

	void setReady(bool ready);

	std::unordered_map<std::string, gl::TexturePtr> textures;
	std::vector<PointLight> pointLights;
	std::vector<DirLight> dirLights;
	std::vector<SpotLight> spotLights;
private:
	void computeShaders();
	void computeShaders(const SceneObjectPtr &node);

	void renderAABB(const SceneObjectPtr &node, const gl::ShaderProgram &program, glm::mat4 accTransform);
	void renderNode(const SceneObjectPtr &node, const MeshShaderPtr &shader, const ShaderConfiguration &configuration, glm::mat4 accTransform);
private:
	SceneObjectPtr rootObject;
	CameraPtr m_camera;

	bool m_ready = false;

	std::unordered_map<std::string, MeshShaderPtr> shaders;

	Arrow m_arrow;
	gl::ShaderProgram m_arrowShader;
};

#endif // !SCENE_H
