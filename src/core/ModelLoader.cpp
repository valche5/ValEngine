#include "ModelLoader.h"

using std::cout;
using std::endl;

std::string ModelLoader::m_directory = "";

ScenePtr ModelLoader::loadScene(const std::string & path)
{
	//Création de la scène
	ScenePtr scene(new Scene);
	SceneObject *rootObject = scene->getRootObject();

	//Chargement du modèle dans assimp
	Assimp::Importer importer;
	const aiScene *aiScene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);


	if (!aiScene || aiScene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !aiScene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
		return ScenePtr(nullptr);
	}
	m_directory = path.substr(0, path.find_last_of('/'));

	processNode(aiScene, aiScene->mRootNode, rootObject, glm::mat4());

	return std::move(scene);
}

void ModelLoader::processNode(const aiScene *aiScene, aiNode * node, SceneObject *object, glm::mat4 accTransform)
{
	//Mesh
	for (int i = 0; i < node->mNumMeshes; i++) {
		aiMesh *mesh = aiScene->mMeshes[node->mMeshes[i]];
		object->meshes.push_back(Mesh());
		processMesh(mesh, &object->meshes.back());
	}
	//Transform
	glm::mat4 nodeTransform = aiToGLM(node->mTransformation);
	glm::mat4 transform = nodeTransform * accTransform;;
	object->transform = transform;
	//Name
	object->name = node->mName.C_Str();

	//Children
	for (int i = 0; i < node->mNumChildren; i++) {
		//Création d'un enfant
		SceneObject *newObject = object->createChild();
		//Parent
		newObject->parent = object;

		processNode(aiScene, node->mChildren[i], newObject, transform);
	}
}

void ModelLoader::processMesh(aiMesh * mesh, Mesh *glMesh)
{
	std::vector<Vertex> &vertices = glMesh->vertices;
	std::vector<GLuint> &indices = glMesh->indices;

	vertices.reserve(mesh->mNumVertices);
	indices.reserve(mesh->mNumFaces);

	//Nom
	glMesh->name = mesh->mName.C_Str();

	//Géométrie
	for (int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		
		vertex.Position = aiToGLM(mesh->mVertices[i]);
		if (mesh->HasNormals()) {
			vertex.Normal = aiToGLM(mesh->mNormals[i]);
		} else {
			vertex.Normal = glm::vec3(0);
		}
			

		if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		} else {
			vertex.TexCoords = glm::vec2(0);
		}

		vertices.push_back(vertex);
	}

	//Indices
	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	//TODO : Material
}

std::vector<Texture> ModelLoader::loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName)
{
	return std::vector<Texture>();
}

glm::mat4 ModelLoader::aiToGLM(const aiMatrix4x4 & mat)
{
	glm::mat4 gmat;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			gmat[i][j] = mat[j][i];
		}
	}

	return gmat;
}

glm::vec3 ModelLoader::aiToGLM(const aiVector3D & vec)
{
	return glm::vec3(vec.x, vec.y, vec.z);
}