#include "Scene.h"

#include <memory>

Scene::Scene() : rootObject(new SceneObject), m_camera(new Camera(glm::vec3(0, 0, 3))) {
	rootObject->parent = nullptr;
}

void Scene::init() {
	computeShaders();
}

SceneObject *Scene::getRootObject() {
	return rootObject.get();
}

Camera * Scene::getCamera() {
	return m_camera.get();
}

void Scene::reloadShaders() {
	for (auto &shader : shaders) {
		shader.second.reload();
	}
}

void Scene::computeShaders() {
	computeShaders(rootObject);
}

void Scene::computeShaders(SceneObjectPtr &node) {
	ShaderConfiguration conf;
	conf.nDirLight = dirLights.size();
	conf.nSpotLight = spotLights.size();
	conf.nPointLight = pointLights.size();

	for (auto &mesh : node->meshes) {
		conf.textureTypes = mesh.material.textureTypes;
		conf.name = mesh.shading;
		//Le shader n'existe pas, on l'ajoute
		if (shaders.find(conf) == shaders.end()) {
			shaders.insert({ conf, PhongShader(conf) });
		}
	}

	for (auto &child : node->childs) {
		computeShaders(child);
	}
}

void Scene::renderNode(SceneObjectPtr & node, const PhongShader &shader, const ShaderConfiguration &configuration, glm::mat4 accTransform) {
	accTransform = node->transform * accTransform;
	bool uSet = false;
	for (auto &mesh : node->meshes) {
		if (mesh.shading == configuration.name
			&& mesh.material.textureTypes == configuration.textureTypes) {
			if (!uSet) {
				shader.setUniforms(accTransform);
				uSet = true;
			}
			shader.setUniforms(mesh.material);
			mesh.draw();
		}
	}

	for (auto &child : node->childs) {
		renderNode(child, shader, configuration, accTransform);
	}
}

void Scene::render() {
	for (auto &shaderpair : shaders) {
		const PhongShader &shader = shaderpair.second;
		ShaderConfiguration configuration = shader.configuration();
		
		
		shader.use();
		shader.setUniforms(*m_camera);

		spotLights[0].position = m_camera->m_position;
		spotLights[0].direction = m_camera->m_front;

		shader.setUniforms(spotLights);
		shader.setUniforms(dirLights);
		shader.setUniforms(pointLights);

		renderNode(rootObject, shader, configuration, glm::mat4());
	}
}
