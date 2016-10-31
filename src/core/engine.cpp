#include "engine.h"

#include <iostream>

using std::cout;
using std::endl;

#include <openGL.h>
#include <camera.h>

#include <Scene.h>

Engine::Engine()
{
}

Engine::~Engine()
{
}

void Engine::init()
{
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		cout << "glew\t\tErreur" << endl;
	} else {
		cout << "glew\t\tOK" << endl;
	}
	if (GLEW_VERSION_4_5) {
		cout << "opengl 4.5\tOK" << endl;
	}

	const GLubyte *GLSLVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	const GLubyte *OGLVersion = glGetString(GL_VERSION);
	cout << "GL Version\t" << OGLVersion << endl;
	cout << "GLSL Version\t" << GLSLVersion << endl;

	glViewport(0, 0, 800, 600);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	m_lastFrameTime = std::chrono::high_resolution_clock::now();

	m_scene = std::make_unique<Scene>();
	m_camera = std::make_unique<Camera>(glm::vec3(0.f, 0.f, 3.0f));
}

void Engine::paint()
{
	//Nettoyage écran openGL
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Calcul du temps entre les frames
	auto frameTime = std::chrono::high_resolution_clock::now();
	float dt = std::chrono::duration_cast<std::chrono::duration<float, std::milli>>(frameTime - m_lastFrameTime).count();
	m_lastFrameTime = frameTime;

	//Mise à jour caméra
	m_camera->update(dt/1000);
	
	//Mise à jour de la vue
	glm::mat4 view = m_camera->GetViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(m_camera->m_zoom), 800.f / 600.f, 0.1f, 100.f);

	//Rendu
	Shader &lighting = m_scene->m_shaders["lighting"];
	lighting.use();
	
	lighting.setUniform("view", view);
	lighting.setUniform("projection", projection);
	lighting.setUniform("viewPos", m_camera->m_position);

	int i = 0;
	for (auto pointLight : m_scene->m_pointLights) {
		pointLight.setUniforms(lighting, i);
		i++;
	}

	m_scene->m_models["nanosuit"].draw(lighting);

	//Shader &lamp = m_scene->m_shaders["lamp"];
	//lamp.use();

	//lamp.setUniform("lampColor", m_scene->m_pointLights[0].diffuse);
	//m_scene->m_models["cube0"].draw(lamp);

	//lamp.setUniform("lampColor", m_scene->m_pointLights[1].diffuse);
	//m_scene->m_models["cube1"].draw(lamp);
}

void Engine::resize(int w, int h)
{
	glViewport(0, 0, h, w);
}

void Engine::clean()
{
}

Camera * Engine::camera()
{
	return m_camera.get();
}

void Engine::reloadShaders()
{
	for (auto &shader : m_scene->m_shaders) {
		cout << "Reload shader " << shader.first << endl;
		shader.second.reload();
	}
}

void Engine::setWireframe(bool enable)
{
	if (enable) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}
