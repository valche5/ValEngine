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
#include <utils/ArrowMesh.h>
#include <utils/AABBMesh.h>

class Scene {
public:
	Scene();
	void init();
	void render();

public:
	SceneObjectPtr &getRootObject();
	CameraPtr getCamera();

	std::string toString();

	std::unordered_map<std::string, gl::TexturePtr> textures;
	std::vector<PointLight> pointLights;
	std::vector<DirLight> dirLights;
	std::vector<SpotLight> spotLights;

	void renderAABB(const SceneObjectPtr &node, const gl::ShaderProgram &program, glm::mat4 accTransform);
private:
	SceneObjectPtr rootObject;
	CameraPtr m_camera;

	ArrowMesh m_arrow;
	AABBMesh m_aabb;
	//gl::ShaderProgram m_arrowShader;
};

#endif // !SCENE_H
