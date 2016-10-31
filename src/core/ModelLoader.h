#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <string>

#include "Scene.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>

class ModelLoader {
public:
	static ScenePtr loadScene(const std::string &path);
private:
	static void processNode(const aiScene *aiScene, aiNode* node, SceneObject *object, glm::mat4 accTransform);
	static void processMesh(aiMesh* mesh, Mesh *glMesh);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
		std::string typeName);

	static std::string m_directory;

	static glm::mat4 aiToGLM(const aiMatrix4x4 &mat);
	static glm::vec3 aiToGLM(const aiVector3D &vec);
};

#endif // MODELLOADER_H
