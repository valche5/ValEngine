#include "Scene.h"

#include <limits>
#include <memory>
#include <cmath>
#include <algorithm>
#include <utils/Glmstreams.h>

Scene::Scene() : rootObject(new SceneObject), m_camera(new Camera(glm::vec3(0, 0, 5))) {
	rootObject->parent = nullptr;
}

void Scene::init() {
	if (dirLights.size() == 0 && pointLights.size() == 0 && spotLights.size() == 0) {
		dirLights.push_back(DirLight());
	}
	//gl::ShaderPtr vertex(new gl::Shader(gl::ShaderType::Vertex));
	//gl::ShaderPtr fragment(new gl::Shader(gl::ShaderType::Fragment));
	//vertex->setSourceFile("shaders/fill.vert");
	//fragment->setSourceFile("shaders/fill.frag");
	//m_arrowShader.attach(vertex);
	//m_arrowShader.attach(fragment);
	//m_arrowShader.compile();
}

SceneObjectPtr &Scene::getRootObject() {
	return rootObject;
}

CameraPtr Scene::getCamera() {
	return m_camera;
}

std::string Scene::toString() {
	return rootObject->toString();
}

void Scene::renderAABB(const SceneObjectPtr & node, const gl::ShaderProgram &program, glm::mat4 accTransform) {
	//accTransform = node->transform * accTransform;
	//for (auto &mesh : node->meshes) {
	//	accTransform = glm::translate(accTransform, mesh->bBox.getCenter());
	//	accTransform = glm::scale(accTransform, mesh->bBox.getSize());
	//	program.setUniform("model", accTransform);
	//	m_aabb.draw();
	//}
	////node->bBox.draw();
	//for (auto &child : node->childs) {
	//	renderAABB(child, program, accTransform);
	//}
}

void Scene::render() {
	//m_arrowShader.use();
	////Z
	//m_arrowShader.setUniform("fillColor", glm::vec3(0, 0, 1));
	//m_arrowShader.setUniform("model", glm::mat4());
	//m_arrowShader.setUniform("view", m_camera->getViewMatrix());
	//m_arrowShader.setUniform("projection", m_camera->getProjection());
	//m_arrow.draw();
	//
	////X
	//m_arrowShader.setUniform("model", glm::rotate(glm::mat4(), glm::radians(-90.f), glm::vec3(1, 0, 0)));
	//m_arrowShader.setUniform("fillColor", glm::vec3(0, 1, 0));
	//m_arrow.draw();

	////Y
	//m_arrowShader.setUniform("model", glm::rotate(glm::mat4(), glm::radians(90.f), glm::vec3(0, 1, 0)));
	//m_arrowShader.setUniform("fillColor", glm::vec3(1, 0, 0));
	//m_arrow.draw();

	//if (1) {
	//	m_arrowShader.setUniform("fillColor", glm::vec3(0.4, 1, 0.4));
	//	renderAABB(rootObject, m_arrowShader, glm::mat4());
	//}
}
