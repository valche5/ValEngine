#include "ModelLoader.h"

#include <core/Mesh.h>
#include <core/VBOFactory.h>
#include <core/IBOFactory.h>

using std::cout;
using std::endl;

std::string ModelLoader::m_directory = "";

ScenePtr ModelLoader::loadScene(const std::string & path) {
	//Création de la scène
	ScenePtr scene(new Scene);
	SceneObject *rootObject = scene->getRootObject();

	//Chargement du modèle dans assimp
	Assimp::Importer importer;
	const aiScene *aiScene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	importer.ApplyPostProcessing(aiProcess_GenNormals);

	if (!aiScene || aiScene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !aiScene->mRootNode) {
		cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
		return ScenePtr(nullptr);
	}
	m_directory = path.substr(0, path.find_last_of('/'));

	processNode(aiScene, scene, aiScene->mRootNode, rootObject);
	scene->setReady(true);

	importer.FreeScene();
	return std::move(scene);
}

void ModelLoader::processNode(const aiScene *aiscene, ScenePtr &scene, aiNode * node, SceneObject *object) {
	//Mesh
	object->meshes.reserve(node->mNumMeshes);
	for (int i = 0; i < node->mNumMeshes; i++) {
		aiMesh *mesh = aiscene->mMeshes[node->mMeshes[i]];
		object->meshes.push_back(MeshPtr(new Mesh));
		processMesh(aiscene, scene, mesh, object->meshes.back());

		//Bounding box
		object->bBox.fitWithAddingBbox(object->meshes.back()->bBox);
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

		//Bounding Box
		object->bBox.fitWithAddingBbox(newObject->bBox);
	}

	object->bBox.initGL();
}

void ModelLoader::processMesh(const aiScene *aiscene, ScenePtr &scene, aiMesh * mesh, MeshPtr &glMesh) {
	//Properties
	glMesh->shading = "phong";
	glMesh->name = mesh->mName.C_Str();
	glMesh->vertexCount = mesh->mNumVertices;

	//Vertex geometry
	VBOFactory vboFactory;
	vboFactory.setVertexCount(mesh->mNumVertices);

	//Position
	vboFactory.setData(VertexPosition, &mesh->mVertices[0][0]);

	//Normals
	if (mesh->HasNormals()) {
		vboFactory.setData(VertexNormal, &mesh->mNormals[0][0]);
	}

	//Texture coords
	std::vector<GLfloat> texCoords;
	if (mesh->HasTextureCoords(0)) {
		//Une petite correction s'impose Vec3 -> Vec2
		texCoords.reserve(2 * mesh->mNumVertices);
		for (int i = 0; i < mesh->mNumVertices; i++) {
			texCoords.push_back(mesh->mTextureCoords[0][i].x);
			texCoords.push_back(mesh->mTextureCoords[0][i].y);
		}
		vboFactory.setData(VertexTexCoord, &texCoords[0]);
	}

	IBOFactory iboFactory;
	//Indices
	std::vector<GLuint> indices;
	indices.reserve(mesh->mNumFaces * 3);
	for (GLuint i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	iboFactory.setData(&indices[0], indices.size());

	//Set mesh VAO and VBO
	glMesh->addVBO(vboFactory.getBlockVBO());
	glMesh->addIBO(iboFactory.getIBO(), indices.size());

	//Material
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial *material = aiscene->mMaterials[mesh->mMaterialIndex];

		//Constants
		aiString name;
		material->Get(AI_MATKEY_NAME, name);
		glMesh->material.name = name.C_Str();

		aiColor3D color(0.f, 0.f, 0.f);
		material->Get(AI_MATKEY_COLOR_AMBIENT, color);
		glMesh->material.addProperty(Ka, aiToGLM(color));

		material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		glMesh->material.addProperty(Kd, aiToGLM(color));

		material->Get(AI_MATKEY_COLOR_SPECULAR, color);
		glMesh->material.addProperty(Ks, aiToGLM(color));

		float shininess = 0;
		material->Get(AI_MATKEY_SHININESS, shininess);
		glMesh->material.addProperty(Shininess, shininess);

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
}

void ModelLoader::loadTextures(ScenePtr &scene, aiMaterial * mat, MeshPtr &glMesh, aiTextureType aitype, TextureType type) {
	if (mat->GetTextureCount(aitype) > 0) {
		aiString str;
		mat->GetTexture(aitype, 0, &str);
		std::string name = str.C_Str();
		//Prevent loading of false textures
		if (name.size() < 4)
			return;

		auto tex = scene->textures.find(name);
		//La texture est elle déja chargée ?
		if (tex == scene->textures.end()) {
			gl::TexturePtr texture(new gl::Texture);
			texture->load(m_directory + "/" + str.C_Str());
			texture->name = name;
			auto inserted = scene->textures.insert({ name, texture });
			glMesh->material.addTexture(type, inserted.first->second);
		} else {
			glMesh->material.addTexture(type, tex->second);
		}

		glMesh->material.textureTypes |= type;

	}
}

glm::mat4 ModelLoader::aiToGLM(const aiMatrix4x4 & mat) {
	glm::mat4 gmat;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			gmat[i][j] = mat[j][i];
		}
	}

	return gmat;
}

glm::vec3 ModelLoader::aiToGLM(const aiVector3D & vec) {
	return glm::vec3(vec.x, vec.y, vec.z);
}

glm::vec3 ModelLoader::aiToGLM(const aiColor3D & color) {
	return glm::vec3(color.r, color.g, color.b);
}
