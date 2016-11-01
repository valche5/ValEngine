#include "PhongShader.h"

#include "Scene.h"

PhongShader::PhongShader(const ShaderConfiguration &configuration) {
	m_configuration = configuration;
	std::string shaderName = m_configuration.name + ".frag";

	m_vertexPath = "lighting_maps.vert";
	m_fragmentPaths.push_back("baseStruct.frag");
	m_fragmentPaths.push_back(shaderName.c_str());

	setDefines();
	load();
}

PhongShader::PhongShader(std::string name, char textureTypes, int nPointLight, int nDirLight, int nSpotLight)
{
	m_configuration.name = name;
	m_configuration.nDirLight = nDirLight;
	m_configuration.nPointLight = nPointLight;
	m_configuration.nSpotLight = nSpotLight;
	m_configuration.textureTypes = textureTypes;

	m_vertexPath = "lighting_maps.vert";
	m_fragmentPaths.push_back("baseStruct.frag");
	m_fragmentPaths.push_back((m_configuration.name + ".frag").c_str());

	setDefines();
	load();
}

void PhongShader::setTextureTypes(char textureTypes) {
	m_configuration.textureTypes = textureTypes;
	setDefines();
	reload();
}

void PhongShader::setNPointLight(int nPointLight) {
	m_configuration.nPointLight = nPointLight;
	setDefines();
	reload();
}

void PhongShader::setNDirLight(int nDirLight) {
	m_configuration.nDirLight = nDirLight;
	setDefines();
	reload();
}

void PhongShader::setNSpotLight(int nSpotLight) {
	m_configuration.nSpotLight = nSpotLight;
	setDefines();
	reload();
}

void PhongShader::setConfiguration(ShaderConfiguration configuration) {
	m_configuration = configuration;
	setDefines();
	reload();
}

void PhongShader::setUniforms(const Camera &camera) const {
	setUniform(m_uView, camera.getViewMatrix());
	setUniform(m_uProjection, camera.getProjection());
	setUniform(m_uViewPos, camera.m_position);
}

void PhongShader::setUniforms(const SceneObject &object) const {
	setUniforms(object.transform);
}

void PhongShader::setUniforms(const glm::mat4 & model) const {
	setUniform(m_uModel, model);
	setUniform(m_uNormalModel, glm::mat3(glm::transpose(glm::inverse(model))));
}

void PhongShader::setUniforms(const Material &mat) const {
	GLuint textureCount = 0;
	if (m_configuration.textureTypes & Ambient) {
		glActiveTexture(GL_TEXTURE0 + textureCount);
		setUniform(m_uKa, textureCount);
		Texture &tex = mat.scene->textures[mat.textures.at(Ambient)];
		tex.bind();
		textureCount++;
	} else {
		setUniform(m_uKa, mat.ka);
	}

	if (m_configuration.textureTypes & Diffuse) {
		glActiveTexture(GL_TEXTURE0 + textureCount);
		setUniform(m_uKd, textureCount);
		Texture &tex = mat.scene->textures[mat.textures.at(Diffuse)];
		tex.bind();
		textureCount++;
	} else {
		setUniform(m_uKd, mat.kd);
	}

	if (m_configuration.textureTypes & Specular) {
		glActiveTexture(GL_TEXTURE0 + textureCount);
		setUniform(m_uKs, textureCount);
		Texture &tex = mat.scene->textures[mat.textures.at(Specular)];
		tex.bind();
		textureCount++;
	} else {
		setUniform(m_uKs, mat.ks);
	}

	if (textureCount > 0) {
		glActiveTexture(GL_TEXTURE0);
	}

	setUniform(m_uShininess, mat.shininess);
}

void PhongShader::setUniforms(const std::vector<PointLight>& pLights) const {
	for (int i = 0; i < m_configuration.nPointLight; i++) {
		setUniform(m_uPointLights[i].ambient, pLights[i].ambient);
		setUniform(m_uPointLights[i].diffuse, pLights[i].diffuse);
		setUniform(m_uPointLights[i].specular, pLights[i].specular);

		setUniform(m_uPointLights[i].constant, pLights[i].constant);
		setUniform(m_uPointLights[i].linear, pLights[i].linear);
		setUniform(m_uPointLights[i].quadratic, pLights[i].quadratic);

		setUniform(m_uPointLights[i].position, pLights[i].position);
	}
}

void PhongShader::setUniforms(const std::vector<DirLight>& dLights) const {
	for (int i = 0; i < m_configuration.nDirLight; i++) {
		setUniform(m_uDirLights[i].ambient, dLights[i].ambient);
		setUniform(m_uDirLights[i].diffuse, dLights[i].diffuse);
		setUniform(m_uDirLights[i].specular, dLights[i].specular);

		setUniform(m_uDirLights[i].direction, dLights[i].direction);
	}
}

void PhongShader::setUniforms(const std::vector<SpotLight>& sLights) const {
	for (int i = 0; i < m_configuration.nSpotLight; i++) {
		setUniform(m_uSpotLights[i].ambient, sLights[i].ambient);
		setUniform(m_uSpotLights[i].diffuse, sLights[i].diffuse);
		setUniform(m_uSpotLights[i].specular, sLights[i].specular);

		setUniform(m_uSpotLights[i].constant, sLights[i].constant);
		setUniform(m_uSpotLights[i].linear, sLights[i].linear);
		setUniform(m_uSpotLights[i].quadratic, sLights[i].quadratic);

		setUniform(m_uSpotLights[i].position, sLights[i].position);
		setUniform(m_uSpotLights[i].direction, sLights[i].direction);

		setUniform(m_uSpotLights[i].cutOff, sLights[i].cutOff);
		setUniform(m_uSpotLights[i].outerCutOff, sLights[i].outerCutOff);
	}
}

void PhongShader::reload() {
	load();
}

ShaderConfiguration PhongShader::configuration() const {
	return m_configuration;
}

void PhongShader::load() {
	Shader::load();
	fillUniformsLoc();
}

void PhongShader::setDefines() {
	m_defines.clear();
	//Lights
	if (m_configuration.nPointLight > 0) {
		m_defines.push_back("N_POINTLIGHT " + std::to_string(m_configuration.nPointLight));
	} else {
		m_defines.push_back("NO_POINTLIGHT");
	}
	if (m_configuration.nDirLight > 0) {
		m_defines.push_back("N_DIRLIGHT " + std::to_string(m_configuration.nDirLight));
	} else {
		m_defines.push_back("NO_DIRLIGHT");
	}
	if (m_configuration.nSpotLight > 0) {
		m_defines.push_back("N_SPOTLIGHT " + std::to_string(m_configuration.nSpotLight));
	} else {
		m_defines.push_back("NO_SPOTLIGHT");
	}
	
	//Textures
	if (m_configuration.textureTypes & Ambient)
		m_defines.push_back("AMBIENT_MAP");
	if (m_configuration.textureTypes & Diffuse)
		m_defines.push_back("DIFFUSE_MAP");
	if (m_configuration.textureTypes & Specular)
		m_defines.push_back("SPECULAR_MAP");
}

void PhongShader::fillUniformsLoc() {
	//Vertex
	m_uModel		= glGetUniformLocation(m_program, "model");
	m_uNormalModel	= glGetUniformLocation(m_program, "normalModel");
	m_uView			= glGetUniformLocation(m_program, "view");
	m_uProjection	= glGetUniformLocation(m_program, "projection");

	//Fragment : Material
	if (m_configuration.textureTypes & Ambient)
		m_uKa = glGetUniformLocation(m_program, "ambient");
	else
		m_uKa = glGetUniformLocation(m_program, "ka");

	if (m_configuration.textureTypes & Diffuse)
		m_uKd = glGetUniformLocation(m_program, "diffuse");
	else
		m_uKd = glGetUniformLocation(m_program, "kd");

	if (m_configuration.textureTypes & Specular)
		m_uKs = glGetUniformLocation(m_program, "specular");
	else
		m_uKs = glGetUniformLocation(m_program, "ks");

	m_uShininess = glGetUniformLocation(m_program, "shininess");

	//Fragment : Lights
	m_uDirLights.clear();
	for (int i = 0; i < m_configuration.nDirLight; i++) {
		std::string n = std::to_string(i);
		uDirLight ulight;
		ulight.ambient		= glGetUniformLocation(m_program, std::string("dirLights[" + n + "].ambient").c_str());
		ulight.diffuse		= glGetUniformLocation(m_program, std::string("dirLights[" + n + "].diffuse").c_str());
		ulight.specular		= glGetUniformLocation(m_program, std::string("dirLights[" + n + "].specular").c_str());
		ulight.direction	= glGetUniformLocation(m_program, std::string("dirLights[" + n + "].direction").c_str());
		m_uDirLights.push_back(ulight);
	}
	m_uSpotLights.clear();
	for (int i = 0; i < m_configuration.nSpotLight; i++) {
		std::string n = std::to_string(i);
		uSpotLight ulight;
		ulight.position	= glGetUniformLocation(m_program, std::string("spotLights[" + n + "].position").c_str());
		ulight.ambient	= glGetUniformLocation(m_program, std::string("spotLights[" + n + "].ambient").c_str());
		ulight.diffuse	= glGetUniformLocation(m_program, std::string("spotLights[" + n + "].diffuse").c_str());
		ulight.specular	= glGetUniformLocation(m_program, std::string("spotLights[" + n + "].specular").c_str());
		ulight.constant	= glGetUniformLocation(m_program, std::string("spotLights[" + n + "].constant").c_str());
		ulight.linear	= glGetUniformLocation(m_program, std::string("spotLights[" + n + "].linear").c_str());
		ulight.quadratic= glGetUniformLocation(m_program, std::string("spotLights[" + n + "].quadratic").c_str());

		ulight.direction= glGetUniformLocation(m_program, std::string("spotLights[" + n + "].direction").c_str());
		ulight.cutOff	= glGetUniformLocation(m_program, std::string("spotLights[" + n + "].cutOff").c_str());
		ulight.outerCutOff= glGetUniformLocation(m_program, std::string("spotLights[" + n + "].cutOff").c_str());
		m_uSpotLights.push_back(ulight);
	}
	m_uPointLights.clear();
	for (int i = 0; i < m_configuration.nPointLight; i++) {
		std::string n = std::to_string(i);
		uPointLight ulight;
		ulight.position	= glGetUniformLocation(m_program, std::string("pointLights[" + n + "].position").c_str());
		ulight.ambient	= glGetUniformLocation(m_program, std::string("pointLights[" + n + "].ambient").c_str());
		ulight.diffuse	= glGetUniformLocation(m_program, std::string("pointLights[" + n + "].diffuse").c_str());
		ulight.specular	= glGetUniformLocation(m_program, std::string("pointLights[" + n + "].specular").c_str());
		ulight.constant	= glGetUniformLocation(m_program, std::string("pointLights[" + n + "].constant").c_str());
		ulight.linear	= glGetUniformLocation(m_program, std::string("pointLights[" + n + "].linear").c_str());
		ulight.quadratic= glGetUniformLocation(m_program, std::string("pointLights[" + n + "].quadratic").c_str());
		m_uPointLights.push_back(ulight);
	}

	//Fragment : viewPos
	m_uViewPos = glGetUniformLocation(m_program, "viewPos");
}
