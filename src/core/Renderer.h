#ifndef RENDERER_H
#define RENDERER_H

#include <core/Scene.h>

typedef struct RenderUnit {
	RenderUnit(MeshShaderPtr meshShader, const ShaderConfiguration &shaderConf)
		: shader(std::move(meshShader)) {
		shader->setConfiguration(shaderConf);
	};
	MeshShaderPtr shader;
	std::vector<Mesh *> meshes;
} RenderUnit;

class Renderer {
public:
	void setScene(ScenePtr scene);
	void clearScene();

	void render();

	void reloadShaders();
	void centerView(const glm::vec3 &dir = glm::vec3(0));
private:
	void createRenderUnits(const SceneObjectPtr &node);

private:
	std::unordered_map<std::string, RenderUnit> m_renderUnits;

	ScenePtr m_scene;
	CameraPtr m_camera;
};

#endif // RENDERER_H
