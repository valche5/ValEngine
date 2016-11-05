#ifndef SHADERCONFIGURATION_H
#define SHADERCONFIGURATION_H

#include <string>

#include "Material.h"

struct ShaderConfiguration {
	std::string name;
	TextureTypes textureTypes;
	int nPointLight;
	int nDirLight;
	int nSpotLight;
	operator std::string() {
		return name + textureTypes;
	}
	friend bool operator<(const ShaderConfiguration &l, const ShaderConfiguration &r) {
		if (l.name < r.name && l.textureTypes < r.textureTypes) {
			return true;
		} else {
			return false;
		}
	};
	friend bool operator==(const ShaderConfiguration &l, const ShaderConfiguration &r) {
		if (l.name == r.name && l.textureTypes == r.textureTypes) {
			return true;
		} else {
			return false;
		}
	};
};

#endif // SHADERCONFIGURATION_H
