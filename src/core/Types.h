#ifndef TYPES_H
#define TYPES_H

#include <memory>

class Renderer;
class Engine;
class Scene;
class Mesh;
class MeshShader;
class Camera;

typedef std::shared_ptr<Camera> CameraPtr;
typedef std::unique_ptr<Renderer> RendererPtr;
typedef std::unique_ptr<Engine> EnginePtr;
typedef std::unique_ptr<Scene> ScenePtr;
typedef std::unique_ptr<Mesh> MeshPtr;
typedef std::unique_ptr<MeshShader> MeshShaderPtr;

#endif // TYPES_H
