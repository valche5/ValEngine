#ifndef PHONGSHADER_H
#define PHONGSHADER_H

#include <vector>

#include "ShaderProgramGL.h"
#include "ShaderConfiguration.h"
#include "camera.h"
#include "Light.h"
#include "Material.h"

class MeshShader {
public:
	MeshShader(const ShaderConfiguration &configuration)
		: m_configuration(configuration), m_vertex(new ShaderGL(ShaderType::Vertex))
		, m_fragment(new ShaderGL(ShaderType::Fragment))
	{}
	virtual ~MeshShader() {};

	void init() {
		m_program.attach(m_vertex);
		m_program.attach(m_fragment);
		load();
		m_program.compile();
		fillUniformsLoc();
	};

	virtual void setUniforms(const Camera &camera) const = 0;
	virtual void setUniforms(const glm::mat4 &model) const = 0;
	virtual void setUniforms(const Material &mat) const = 0;
	virtual void setUniforms(const std::vector<PointLight> &pLights) const = 0;
	virtual void setUniforms(const std::vector<DirLight> &dLights) const = 0;
	virtual void setUniforms(const std::vector<SpotLight> &sLights) const = 0;

	void use() const { m_program.use(); };
	void reload() const { m_program.compile(); };

	ShaderConfiguration configuration() const {
		return m_configuration;
	}
protected:
	virtual void load() = 0;
	virtual void fillUniformsLoc() = 0;

protected:
	//ShaderProgram
	ShaderProgramGL m_program;
	ShaderGLPtr m_vertex;
	ShaderGLPtr m_fragment;

	//Configuration
	ShaderConfiguration m_configuration;
};

typedef std::unique_ptr<MeshShader> MeshShaderPtr;

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

class LightingShader : public MeshShader {
public:
	LightingShader(const ShaderConfiguration &conf)
		: MeshShader(conf) {};

	virtual void setUniforms(const Camera &camera) const;
	virtual void setUniforms(const glm::mat4 &model) const;
	virtual void setUniforms(const Material &mat) const;
	virtual void setUniforms(const std::vector<PointLight> &pLights) const;
	virtual void setUniforms(const std::vector<DirLight> &dLights) const;
	virtual void setUniforms(const std::vector<SpotLight> &sLights) const;
protected:
	virtual void load();
	virtual void fillUniformsLoc();

private:
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
