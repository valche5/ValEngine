#ifndef SHADERCOMPONENT_H
#define SHADERCOMPONENT_H

#include "Camera.h"
#include "Light.h"
#include "Mesh.h"

struct uPointLight {
	GLuint position;
	GLuint constant;
	GLuint linear;
	GLuint quadratic;
	GLuint ambient;
	GLuint diffuse;
	GLuint specular;
};

struct uDirLight {
	GLuint direction;
	GLuint ambient;
	GLuint diffuse;
	GLuint specular;
};

struct uSpotLight {
	GLuint position;
	GLuint direction;
	GLuint cutOff;
	GLuint outerCutOff;
	GLuint constant;
	GLuint linear;
	GLuint quadratic;
	GLuint ambient;
	GLuint diffuse;
	GLuint specular;
};

class ShaderComponent {
public:
	virtual void updateCamera(const Camera &camera) const = 0;
	virtual void updateMesh(const Mesh &mesh) const = 0;
	virtual void updatePointLights(const std::vector<PointLight> &pLights) const = 0;
	virtual void updateDirLights(const std::vector<DirLight> &dLights) const = 0;
	virtual void updateSpotLights(const std::vector<SpotLight> &sLights) const = 0;

	virtual void fillUniformsLoc() = 0;

	virtual std::string name() = 0;
	virtual std::string configuration() = 0;
};

class SamplerShaderComponent : public ShaderComponent {
public:
	virtual void updateCamera(const Camera &camera) const;
	virtual void updateMesh(const Mesh &mesh) const;
	virtual void updatePointLights(const std::vector<PointLight> &pLights) const;
	virtual void updateDirLights(const std::vector<DirLight> &dLights) const;
	virtual void updateSpotLights(const std::vector<SpotLight> &sLights) const;

	virtual void fillUniformsLoc();

	virtual std::string name() = 0;
	virtual std::string configuration() = 0;
};

class ProjectiveShaderComponent : public ShaderComponent {
public:
	virtual void updateCamera(const Camera &camera) const;
	virtual void updateMesh(const Mesh &mesh) const;
	virtual void updatePointLights(const std::vector<PointLight> &pLights) const;
	virtual void updateDirLights(const std::vector<DirLight> &dLights) const;
	virtual void updateSpotLights(const std::vector<SpotLight> &sLights) const;

	virtual void fillUniformsLoc();

	virtual std::string name() = 0;
	virtual std::string configuration() = 0;
private:
	//Vertex Shader
	GLuint m_uModel;
	GLuint m_uNormalModel;
	GLuint m_uView;
	GLuint m_uProjection;

	//Fragment Shader : View
	GLuint m_uViewPos;
};

class LightingShaderComponent : public ShaderComponent {
public:
	LightingShaderComponent(int pointLightCount, int dirLightCount, int spotLightCount);

	void setPointLightCount(int pointLightCount);
	void setDirLightCount(int dirLightCount);
	void setSpotLightCount(int spotLightCount);

	virtual void updateCamera(const Camera &camera) const;
	virtual void updateMesh(const Mesh &mesh) const;
	virtual void updatePointLights(const std::vector<PointLight> &pLights) const;
	virtual void updateDirLights(const std::vector<DirLight> &dLights) const;
	virtual void updateSpotLights(const std::vector<SpotLight> &sLights) const;

	virtual void fillUniformsLoc();

	virtual std::string name() = 0;
	virtual std::string configuration() = 0;
private:
	//Fragment Shader : Lights
	std::vector<uPointLight> m_uPointLights;
	std::vector<uDirLight> m_uDirLights;
	std::vector<uSpotLight> m_uSpotLights;
};


#endif // SHADERCOMPONENT_H
