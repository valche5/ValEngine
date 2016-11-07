#include "Scene.h"

#include <memory>

Scene::Scene() : rootObject(new SceneObject), m_camera(new Camera(glm::vec3(0, 0, 5))) {
	rootObject->parent = nullptr;
}

void Scene::init() {
	if (m_ready)
		computeShaders();

	gl::ShaderPtr vertex(new gl::Shader(gl::ShaderType::Vertex));
	gl::ShaderPtr fragment(new gl::Shader(gl::ShaderType::Fragment));
	vertex->setSourceFile("shaders/fill.vert");
	fragment->setSourceFile("shaders/fill.frag");
	m_arrowShader.attach(vertex);
	m_arrowShader.attach(fragment);
	m_arrowShader.compile();
}

SceneObject *Scene::getRootObject() {
	return rootObject.get();
}

Camera * Scene::getCamera() {
	return m_camera.get();
}

void Scene::reloadShaders() {
	for (auto &shader : shaders) {
		shader.second->reload();
	}
}

void Scene::setReady(bool ready) {
	m_ready = ready;
}

void Scene::computeShaders() {
	computeShaders(rootObject);
}

void Scene::computeShaders(const SceneObjectPtr &node) {
	ShaderConfiguration conf;
	conf.nDirLight = dirLights.size();
	conf.nSpotLight = spotLights.size();
	conf.nPointLight = pointLights.size();

	for (auto &mesh : node->meshes) {
		conf.textureTypes = mesh->material.textureTypes;
		conf.name = mesh->shading;
		//Le shader n'existe pas, on l'ajoute
		if (shaders.find(conf) == shaders.end()) {
			shaders.insert({ conf, MeshShaderPtr(new LightingShader(conf)) });
			shaders.at(conf)->init();
		}
	}

	for (const auto &child : node->childs) {
		computeShaders(child);
	}
}

void Scene::renderAABB(const SceneObjectPtr & node, const gl::ShaderProgram &program, glm::mat4 accTransform) {
	accTransform = node->transform * accTransform;
	program.setUniform("model", accTransform);
	for (auto &mesh : node->meshes) {
		mesh->bBox.draw();
	}
	node->bBox.draw();
	for (auto &child : node->childs) {
		renderAABB(child, program, accTransform);
	}
}

void Scene::renderNode(const SceneObjectPtr & node, const MeshShaderPtr &shader, const ShaderConfiguration &configuration, glm::mat4 accTransform) {
	accTransform = node->transform * accTransform;
	bool uSet = false;
	for (auto &mesh : node->meshes) {
		if (mesh->shading == configuration.name
			&& mesh->material.textureTypes == configuration.textureTypes) {
			if (!uSet) {
				shader->setUniforms(accTransform);
				uSet = true;
			}
			shader->setUniforms(mesh->material);
			mesh->draw();
		}
	}

	for (auto &child : node->childs) {
		renderNode(child, shader, configuration, accTransform);
	}
}

void Scene::render() {
	if (m_ready) {
		for (auto &shaderpair : shaders) {
			const MeshShaderPtr &shader = shaderpair.second;
			ShaderConfiguration configuration = shader->configuration();


			shader->use();
			shader->setUniforms(*m_camera);

			//spotLights[0].position = m_camera->m_position;
			//spotLights[0].direction = m_camera->m_front;

			shader->setUniforms(spotLights);
			shader->setUniforms(dirLights);
			shader->setUniforms(pointLights);

			renderNode(rootObject, shader, configuration, glm::mat4());
		}
	}

	m_arrowShader.use();
	//Z
	m_arrowShader.setUniform("fillColor", glm::vec3(0, 0, 1));
	m_arrowShader.setUniform("model", glm::mat4());
	m_arrowShader.setUniform("view", m_camera->getViewMatrix());
	m_arrowShader.setUniform("projection", m_camera->getProjection());
	m_arrow.draw();
	
	//X
	m_arrowShader.setUniform("model", glm::rotate(glm::mat4(), glm::radians(-90.f), glm::vec3(1, 0, 0)));
	m_arrowShader.setUniform("fillColor", glm::vec3(0, 1, 0));
	m_arrow.draw();

	//Y
	m_arrowShader.setUniform("model", glm::rotate(glm::mat4(), glm::radians(90.f), glm::vec3(0, 1, 0)));
	m_arrowShader.setUniform("fillColor", glm::vec3(1, 0, 0));
	m_arrow.draw();

	if (0) {
		m_arrowShader.setUniform("fillColor", glm::vec3(0.4, 1, 0.4));
		renderAABB(rootObject, m_arrowShader, glm::mat4());
	}
}
