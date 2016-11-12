#include "Engine.h"

#include <iostream>

using std::cout;
using std::endl;

#include <core/openGL.h>
#include <core/Camera.h>
#include <core/ModelLoader.h>
#include <core/Renderer.h>
#include <core/Scene.h>
#include <utils/debugMessage.h>

Engine::Engine()
	: m_renderer(new Renderer)
{}

Engine::~Engine()
{
}

void Engine::init()
{
	glewExperimental = GL_TRUE;
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

	//Debug
	GLint flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
		cout << "Debug context\tOK" << endl;
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(debugMessage, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}

	m_camera = CameraPtr(new Camera(glm::vec3(0, 0, 5)));

	glViewport(0, 0, 800, 600);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	m_lastFrameTime = std::chrono::high_resolution_clock::now();
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
	
	m_renderer->render();
}

void Engine::resize(int w, int h)
{
	glViewport(0, 0, w, h);
	m_screenSize.x = w;
	m_screenSize.y = h;
	m_camera->setScreenSize(w, h);
}

CameraPtr Engine::camera()
{
	return m_camera;
}

void Engine::closeScene() {
	m_renderer->clearScene();
	m_camera = CameraPtr(new Camera(glm::vec3(0, 0, 5)));
}

void Engine::loadScene(const std::string & path) {
	try {
		ScenePtr scene = ModelLoader::loadScene(path);
		scene->init();

		cout << scene->toString() << endl;

		m_camera = scene->getCamera();
		m_camera->setScreenSize(m_screenSize.x, m_screenSize.y);

		m_renderer->setScene(std::move(scene));
	} catch (const std::exception &e) {
		std::cout << "Impossible de charger la scène : " << e.what() << std::endl;
	}

}

void Engine::centerScene(const glm::vec3 &dir) {
	m_renderer->centerView(dir);
}

void Engine::reloadShaders()
{
	m_renderer->reloadShaders();
}

void Engine::setWireframe(bool enable)
{
	if (enable) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}
