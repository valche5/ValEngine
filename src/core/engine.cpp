#include "Engine.h"

#include <iostream>

using std::cout;
using std::endl;

#include <core/openGL.h>
#include <core/Camera.h>
#include <core/ModelLoader.h>
#include <utils/debugMessage.h>

Engine::Engine()
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

	//m_scene = ModelLoader::loadScene("data/crysis/nanosuit.obj");
	loadDefaultScene();

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
	
	m_scene->render();
}

void Engine::resize(int w, int h)
{
	glViewport(0, 0, w, h);
	m_screenSize.x = w;
	m_screenSize.y = h;
	m_camera->setScreenSize(w, h);
}

void Engine::clean()
{
}

Camera * Engine::camera()
{
	return m_camera;
}

void Engine::loadDefaultScene() {
	m_scene = ScenePtr(new Scene);
	m_scene->init();
	m_camera = m_scene->getCamera();
}

void Engine::loadScene(const std::string & path) {
	m_scene = ModelLoader::loadScene(path);

	cout << m_scene->toString() << endl;

	glm::vec3 center = m_scene->getRootObject()->bBox.getCenter();
	cout << "Scene Center : (" << center.x << "," << center.y << "," << center.z << ")" << endl;

	m_camera = m_scene->getCamera();
	m_camera->setScreenSize(m_screenSize.x, m_screenSize.y);

	m_scene->dirLights.push_back(DirLight());
	PointLight light(glm::vec3(0, 3, 0));
	m_scene->pointLights.push_back(light);

	//SpotLight slight;
	//m_scene->spotLights.push_back(slight);

	m_scene->init();
}

void Engine::centerScene(const glm::vec3 &dir) {
	m_camera->centerOnAABB(m_scene->getRootObject()->bBox, dir);
}

void Engine::reloadShaders()
{
	m_scene->reloadShaders();
}

void Engine::setWireframe(bool enable)
{
	if (enable) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}
