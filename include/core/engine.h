#ifndef ENGINE_H
#define ENGINE_H

#include <memory>
#include <chrono>

#include <camera.h>

class Scene;

typedef std::chrono::high_resolution_clock::time_point timepoint;

class Engine {
public:
	Engine();
	~Engine();

	void init();
	void paint();
	void resize(int w, int h);
	void clean();

	Camera *camera();

	void reloadShaders();
	void setWireframe(bool enable);

	
private:
	unsigned int m_shaderProgram;
	unsigned int m_VAO;

	timepoint m_lastFrameTime;

	std::unique_ptr<Camera> m_camera;
	std::unique_ptr<Scene> m_scene;
};

#endif // !ENGINE_H
