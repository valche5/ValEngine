#include "Renderer.h"

void Renderer::setScene(ScenePtr scene) {
	m_renderUnits.clear();
	m_scene = std::move(scene);
	m_camera = m_scene->getCamera();
	createRenderUnits(m_scene->getRootObject());
	m_camera->centerOnAABB(m_scene->getRootObject()->bBox);
}

void Renderer::clearScene() {
	m_scene.reset();
	m_renderUnits.clear();
}

void Renderer::render() {
	for (auto &renderUnitpair : m_renderUnits) {
		RenderUnit &renderUnit = renderUnitpair.second;
		MeshShaderPtr &shader = renderUnit.shader;

		shader->use();
		shader->setUniforms(*m_scene->getCamera());

		shader->setUniforms(m_scene->spotLights);
		shader->setUniforms(m_scene->dirLights);
		shader->setUniforms(m_scene->pointLights);

		for (auto &mesh : renderUnit.meshes) {
			shader->setUniforms(mesh->model);
			shader->setUniforms(mesh->material);
			mesh->draw();
		}

		shader->unbind();
	}
}

void Renderer::reloadShaders() {
	for (auto &ru : m_renderUnits) {
		ru.second.shader->reload();
	}
}

void Renderer::centerView(const glm::vec3 & dir) {
	m_scene->getCamera()->centerOnAABB(m_scene->getRootObject()->bBox, dir);
}

void Renderer::createRenderUnits(const SceneObjectPtr &node) {
	for (auto &mesh : node->meshes) {
		const ShaderConfiguration &shaderConf = mesh->shaderConf;
		//Le shader n'existe pas, on l'ajoute
		if (m_renderUnits.find(shaderConf.toString()) == m_renderUnits.end()) {
			if (shaderConf.shadingType == "lighting") {
				RenderUnit ru(MeshShaderPtr(
					new LightingShader(m_scene->pointLights.size(), m_scene->spotLights.size(), m_scene->dirLights.size())),
					shaderConf
				);
				m_renderUnits.insert({ shaderConf.toString(), std::move(ru) });
			} else if (mesh->shaderConf.shadingType == "fill") {
				RenderUnit ru(MeshShaderPtr(
					new FillShader()),
					shaderConf
				);
				m_renderUnits.insert({ shaderConf.toString(), std::move(ru) });
			}
		}

		m_renderUnits.at(shaderConf.toString()).meshes.push_back(mesh.get());
	}

	for (const auto &child : node->childs) {
		createRenderUnits(child);
	}
}
