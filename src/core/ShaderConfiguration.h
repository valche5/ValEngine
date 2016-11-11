#ifndef SHADERCONFIGURATION_H
#define SHADERCONFIGURATION_H

#include <string>

#include <core/Material.h>

struct ShaderConfiguration {
	ShaderConfiguration(const std::string &shadingType = "fill", const std::string &lightingFct = {}, TextureTypes textureTypes = 0)
		: shadingType(shadingType), lightingFct(lightingFct), textureTypes(textureTypes) {
	};
	std::string shadingType;
	std::string lightingFct;
	TextureTypes textureTypes;
	operator std::string() {
		return shadingType + lightingFct + textureTypes;
	}
	friend bool operator<(const ShaderConfiguration &l, const ShaderConfiguration &r) {
		if (l.lightingFct < r.lightingFct && l.shadingType < r.shadingType && l.textureTypes < r.textureTypes) {
			return true;
		} else {
			return false;
		}
	};
	friend bool operator==(const ShaderConfiguration &l, const ShaderConfiguration &r) {
		if (l.lightingFct == r.lightingFct && l.shadingType == r.shadingType && l.textureTypes == r.textureTypes) {
			return true;
		} else {
			return false;
		}
	};
};

#endif // SHADERCONFIGURATION_H
