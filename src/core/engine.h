#ifndef ENGINE_H
#define ENGINE_H

#include <memory>
#include <chrono>
#include <string>

#include <core/Types.h>
#include <glm/glm.hpp>
#include <core/Camera.h>

typedef std::chrono::high_resolution_clock::time_point timepoint;

class Engine {
public:
	Engine();
	~Engine();

	void init();
	void paint();
	void resize(int w, int h);

	CameraPtr camera();

	void closeScene();
	void loadScene(const std::string &path);
	void centerScene(const glm::vec3 &dir = glm::vec3(0));

	void reloadShaders();
	void setWireframe(bool enable);
private:
	unsigned int m_shaderProgram;
	unsigned int m_VAO;

	timepoint m_lastFrameTime;

	glm::vec3 m_screenSize;

	RendererPtr m_renderer;
	CameraPtr m_camera;
};

#endif // !ENGINE_H
