#ifndef SCENE_H
#define SCENE_H

#include <unordered_map>

#include <model.h>
#include <shader.h>
#include <light.h>

class Scene {
public:
	Scene() {
		m_models["nanosuit"].load("data/crysis/nanosuit.obj");
		m_models["cube0"].load("data/cube/cube.obj");
		m_models["cube1"].load("data/cube/cube.obj");

		//m_pointLights.push_back(PointLight(glm::vec3(0.7, 0.2, 2.0)));
		m_pointLights.push_back(PointLight(glm::vec3(0, 0, 3.0)));
		PointLight &l = m_pointLights[0];
		l.diffuse = glm::vec3(1.0);
		l.specular = glm::vec3(1.0);
		l.ambient = glm::vec3(0.3);

		m_models["cube0"].translate(m_pointLights[0].position);
		//m_models["cube1"].translate(m_pointLights[1].position);

		Model &nano = m_models["nanosuit"];
		nano.model = glm::translate(nano.model, glm::vec3(0, -1.75, 0));
		nano.model = glm::scale(nano.model, glm::vec3(0.1));

		std::vector<std::string> defines;
		defines.push_back("NR_POINT_LIGHTS " + std::to_string(m_pointLights.size()));
		m_shaders["lighting"].load("shaders/lighting_maps.vert", "shaders/lighting_maps.frag", defines);

		m_shaders["lamp"].load("shaders/lamp.vert", "shaders/lamp.frag");
	}
	void init() {

	}
	std::unordered_map<std::string, Model> m_models;
	std::unordered_map<std::string, Shader> m_shaders;
	std::vector<PointLight> m_pointLights;

	GLuint VAO;
	GLuint VBO;
};

#endif // !SCENE_H
