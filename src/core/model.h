#ifndef MODEL_H
#define MODEL_H

#include "shader.h"
#include "mesh.h"
#include "SceneObject.h"

#include <vector>
#include <string>

#include "OpenGL.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
    public:
        /*  Functions   */
        void load(const std::string &path);
        void draw(Shader shader);

		void setPosition(glm::vec3 position);
		void translate(glm::vec3 translation);
		void scale(glm::vec3 scaling);

		glm::mat4 model;
    private:
        /*  Model Data  */
        std::vector<Mesh> meshes;
        std::string directory;

        /*  Functions   */
        void loadModel(const std::string &path);
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
                                             std::string typeName);
};

#endif // MODEL_H
