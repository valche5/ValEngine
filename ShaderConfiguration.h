#ifndef SHADERCONFIGURATION_H
#define SHADERCONFIGURATION_H

#include <string>

struct ShaderConfiguration {
	std::string name;
	char textureTypes;
	int nPointLight;
	int nDirLight;
	int nSpotLight;
	operator std::string() {
		return name + textureTypes + std::to_string(nPointLight) + std::to_string(nDirLight) + std::to_string(nSpotLight);
	}
};

//bool operator<(const ShaderConfiguration &l, const ShaderConfiguration &r) {
//	if (l.name < r.name) {
//		return true;
//	} else if (l.textureTypes < r.textureTypes) {
//		return true;
//	} else if (l.nPointLight < r.nPointLight) {
//		return true;
//	} else if (l.nDirLight < r.nDirLight) {
//		return true;
//	} else if (l.nSpotLight < r.nSpotLight) {
//		return true;
//	}
//};
//
//bool operator==(const ShaderConfiguration &l, const ShaderConfiguration &r) {
//	if (l.name == r.name && l.textureTypes == r.textureTypes
//		&& l.nPointLight == r.nPointLight && l.nDirLight == r.nDirLight
//		&& l.nSpotLight == r.nSpotLight) {
//		return true;
//	}
//};

#endif // SHADERCONFIGURATION_H
