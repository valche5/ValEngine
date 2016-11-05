#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

class Scene;

typedef char TextureTypes;

enum TextureType : TextureTypes {
	Ambient = 0x01,
	Diffuse = 0x02,
	Specular = 0x04,
	Normal = 0x08
};

const std::unordered_map<TextureType, std::string> textureTypeString({
	{ Ambient, "Ambient" },
	{ Diffuse, "Diffuse" },
	{ Specular, "Specular" },
	{ Normal, "Normal" }
});

class Material {
public:
	TextureTypes textureTypes = 0x00;

	std::unordered_map<TextureType, std::string> textures;

	glm::vec3 ka = glm::vec3(0, 0, 0);
	glm::vec3 kd = glm::vec3(0, 0, 0);
	glm::vec3 ks = glm::vec3(0, 0, 0);

	float shininess = 0.f;

	std::string name;

	Scene *scene;
};

#endif // MATERIAL_H
