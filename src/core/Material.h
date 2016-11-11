#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

#include <glw/Types.h>

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

enum MaterialProperty {
	Ka, Kd, Ks, Shininess, FillColor
};

class Material {
public:
	std::string name;
	TextureTypes textureTypes = 0x00;
public:
	void addTexture(TextureType type, gl::TexturePtr texture) {
		textureTypes |= type;
		m_textures.insert({ type, texture });
	}

	gl::TexturePtr &getTexture(TextureType type) {
		return m_textures.at(type);
	}

	const gl::TexturePtr &getTexture(TextureType type) const {
		return m_textures.at(type);
	}

	std::unordered_map<TextureType, gl::TexturePtr> &getTextures() {
		return m_textures;
	}

	const std::unordered_map<TextureType, gl::TexturePtr> &getTextures() const {
		return m_textures;
	}

	template<typename T>
	void addProperty(MaterialProperty prop, T value) {
		std::unordered_map<MaterialProperty, T> &map = getMap<T>();
		map[prop] = value;
	};

	template<typename T>
	void setProperty(MaterialProperty prop, T value) {
		std::unordered_map<MaterialProperty, T> &map = getMap<T>();
		map[prop] = value;
	};

	template<typename T>
	bool issetProperty(MaterialProperty prop) const {
		const std::unordered_map<MaterialProperty, T> &map = getMap<T>();
		return (map.find(prop) == map.end()) false : true;
	};

	template<typename T>
	T getProperty(MaterialProperty prop) const {
		const std::unordered_map<MaterialProperty, T> &map = getMap<T>();
		return map.at(prop);
	}

private:
	template<typename T>
	std::unordered_map<MaterialProperty, T> &getMap() {
		return std::unordered_map<MaterialProperty, T>();
	};
	template<>
	std::unordered_map<MaterialProperty, float> &getMap<float>() {
		return m_floatProperties;
	}
	template<>
	std::unordered_map<MaterialProperty, glm::vec3> &getMap<glm::vec3>() {
		return m_vec3Properties;
	}
	template<typename T>
	const std::unordered_map<MaterialProperty, T> &getMap() const {
		return std::unordered_map<MaterialProperty, T>();
	};
	template<>
	const std::unordered_map<MaterialProperty, float> &getMap<float>() const {
		return m_floatProperties;
	}
	template<>
	const std::unordered_map<MaterialProperty, glm::vec3> &getMap<glm::vec3>() const {
		return m_vec3Properties;
	}  
private:
	std::unordered_map<TextureType, gl::TexturePtr> m_textures;

	std::unordered_map<MaterialProperty, float> m_floatProperties;
	std::unordered_map<MaterialProperty, glm::vec3> m_vec3Properties;
};

#endif // MATERIAL_H
