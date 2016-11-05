#include "ModelLoader.h"

#include "Mesh.h"

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
	importer.ApplyPostProcessing(aiProcess_GenNormals);

	if (!aiScene || aiScene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !aiScene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
		return ScenePtr(nullptr);
	}
	m_directory = path.substr(0, path.find_last_of('/'));

	processNode(aiScene, scene, aiScene->mRootNode, rootObject);

	return std::move(scene);
}

void ModelLoader::processNode(const aiScene *aiscene, ScenePtr &scene, aiNode * node, SceneObject *object)
{
	//Mesh
	for (int i = 0; i < node->mNumMeshes; i++) {
		aiMesh *mesh = aiscene->mMeshes[node->mMeshes[i]];
		object->meshes.push_back(Mesh());
		processMesh(aiscene, scene, mesh, &object->meshes.back());
	}
	//Transform
	object->transform = aiToGLM(node->mTransformation);
	//Name
	object->name = node->mName.C_Str();

	//Children
	for (int i = 0; i < node->mNumChildren; i++) {
		//Création d'un enfant
		SceneObject *newObject = object->createChild();
		//Parent
		newObject->parent = object;
		newObject->scene = scene.get();

		processNode(aiscene, scene, node->mChildren[i], newObject);
	}
}

void ModelLoader::processMesh(const aiScene *aiscene, ScenePtr &scene, aiMesh * mesh, Mesh *glMesh)
{
	std::vector<Vertex> &vertices = glMesh->vertices;
	std::vector<GLuint> &indices = glMesh->indices;

	//Set shading
	glMesh->shading = "phong";

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

	//Material
	glMesh->material.scene = scene.get(); //Ref to the scene
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial *material = aiscene->mMaterials[mesh->mMaterialIndex];

		//Constants
		aiString name;
		material->Get(AI_MATKEY_NAME, name);
		glMesh->material.name = name.C_Str();

		aiColor3D color(0.f, 0.f, 0.f);
		material->Get(AI_MATKEY_COLOR_AMBIENT, color);
		glMesh->material.ka = aiToGLM(color);

		material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		glMesh->material.kd = aiToGLM(color);

		material->Get(AI_MATKEY_COLOR_SPECULAR, color);
		glMesh->material.ks = aiToGLM(color);

		float shininess = 0;
		material->Get(AI_MATKEY_SHININESS, shininess);
		glMesh->material.shininess = shininess;

		//Textures
		loadTextures(scene, material, glMesh, aiTextureType_AMBIENT, TextureType::Ambient);
		loadTextures(scene, material, glMesh, aiTextureType_DIFFUSE, TextureType::Diffuse);
		loadTextures(scene, material, glMesh, aiTextureType_SPECULAR, TextureType::Specular);
		loadTextures(scene, material, glMesh, aiTextureType_NORMALS, TextureType::Normal);
		//La Height Map peut être la Normal map
		if (!(glMesh->material.textureTypes & TextureType::Normal)) {
			loadTextures(scene, material, glMesh, aiTextureType_HEIGHT, TextureType::Normal);
		}

		//Si pas de diffuse map mais une ambient map, ambient map = diffuse map
		if ((glMesh->material.textureTypes & Diffuse) && !(glMesh->material.textureTypes & Ambient)) {
			loadTextures(scene, material, glMesh, aiTextureType_DIFFUSE, TextureType::Ambient);
		}
	}

	glMesh->setupMesh();
}

void ModelLoader::loadTextures(ScenePtr &scene, aiMaterial * mat, Mesh *glMesh, aiTextureType aitype, TextureType type)
{
	if (mat->GetTextureCount(aitype) > 0) {
		aiString str;
		mat->GetTexture(aitype, 0, &str);
		std::string name = str.C_Str();
		//La texture est elle déja chargée ?
		if (scene->textures.find(name) == scene->textures.end()) {
			Texture texture;
			texture.load(m_directory + "/" + str.C_Str());
			scene->textures.insert({name, texture});
		}
		glMesh->material.textureTypes |= type;
		glMesh->material.textures.insert({ type, name });
	}
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

glm::vec3 ModelLoader::aiToGLM(const aiColor3D & color)
{
	return glm::vec3(color.r, color.g, color.b);
}
