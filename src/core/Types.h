#ifndef TYPES_H
#define TYPES_H

#include <memory>

class Scene;
class Mesh;
class MeshShader;

typedef std::unique_ptr<Scene> ScenePtr;
typedef std::unique_ptr<Mesh> MeshPtr;
typedef std::unique_ptr<MeshShader> MeshShaderPtr;

#endif // TYPES_H
