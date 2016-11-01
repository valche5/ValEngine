#ifndef PHONGSHADER_H
#define PHONGSHADER_H

#include <vector>

#include "shader.h"
#include "ShaderConfiguration.h"
#include "camera.h"
#include "SceneObject.h"
#include "Light.h"
#include "Material.h"

class PhongShader : public Shader {
public:
	PhongShader(const ShaderConfiguration &configuration);
	PhongShader(std::string name, char textureTypes, int nPointLight, int nDirLight, int nSpotLight);
	

	void setTextureTypes(char textureTypes);
	void setNPointLight(int nPointLight);
	void setNDirLight(int nDirLight);
	void setNSpotLight(int nSpotLight);
	void setConfiguration(ShaderConfiguration configuration);

	void setUniforms(const Camera &camera) const;
	void setUniforms(const SceneObject &object) const;
	void setUniforms(const glm::mat4 &model) const;
	void setUniforms(const Material &mat) const;
	void setUniforms(const std::vector<PointLight> &pLights) const;
	void setUniforms(const std::vector<DirLight> &dLights) const;
	void setUniforms(const std::vector<SpotLight> &sLights) const;

	void reload();

	ShaderConfiguration configuration() const;
private:
	void load();
	void setDefines();
	void fillUniformsLoc();

private:
	//Configuration
	ShaderConfiguration m_configuration;

	//Vertex Shader
	GLuint m_uModel;
	GLuint m_uNormalModel;
	GLuint m_uView;
	GLuint m_uProjection;

	//Fragment Shader : Material
	GLuint m_uKa;
	GLuint m_uKd;
	GLuint m_uKs;
	GLuint m_uShininess;

	//Fragment Shader : Lights
	std::vector<uPointLight> m_uPointLights;
	std::vector<uDirLight> m_uDirLights;
	std::vector<uSpotLight> m_uSpotLights;

	//Fragment Shader : View
	GLuint m_uViewPos;
};

#endif // PHONGSHADER_H
