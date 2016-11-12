#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>

#include <core/Scene.h>
#include <core/Types.h>

class ModelLoader {
public:
	static ScenePtr loadScene(const std::string &path);
private:
	static void processNode(const aiScene *aiscene, ScenePtr &scene, aiNode* node, SceneObjectPtr &object, glm::mat4 accTransform);
	static void processMesh(const aiScene *aiscene, ScenePtr &scene, aiMesh* mesh, MeshPtr &glMesh, const glm::mat4 &model);
	static void loadTextures(ScenePtr &scene, aiMaterial* mat, MeshPtr &glMesh, aiTextureType aitype, TextureType type);

	static void computeAABB(aiMesh *mesh, AABB &bBox);

	static std::string m_directory;

	static glm::mat4 aiToGLM(const aiMatrix4x4 &mat);
	static glm::vec3 aiToGLM(const aiVector3D &vec);
	static glm::vec3 aiToGLM(const aiColor3D &color);
};

#endif // MODELLOADER_H
