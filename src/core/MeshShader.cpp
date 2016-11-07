#include "MeshShader.h"

#include <core/Scene.h>

void LightingShader::setUniforms(const Camera &camera) const {
	m_program.setUniform(m_uView, camera.getViewMatrix());
	m_program.setUniform(m_uProjection, camera.getProjection());
	m_program.setUniform(m_uViewPos, camera.m_position);
}

void LightingShader::setUniforms(const glm::mat4 & model) const {
	m_program.setUniform(m_uModel, model);
	m_program.setUniform(m_uNormalModel, glm::mat3(glm::transpose(glm::inverse(model))));
}

void LightingShader::setUniforms(const Material &mat) const {
	GLuint textureCount = 0;
	if (m_configuration.textureTypes & Ambient) {
		glActiveTexture(GL_TEXTURE0 + textureCount);
		m_program.setUniform(m_uKa, textureCount);
		mat.getTexture(Ambient)->bind();
		textureCount++;
	} else {
		m_program.setUniform(m_uKa, mat.getProperty<glm::vec3>(Ka));
	}

	if (m_configuration.textureTypes & Diffuse) {
		glActiveTexture(GL_TEXTURE0 + textureCount);
		m_program.setUniform(m_uKd, textureCount);
		mat.getTexture(Diffuse)->bind();
		textureCount++;
	} else {
		m_program.setUniform(m_uKd, mat.getProperty<glm::vec3>(Kd));
	}

	if (m_configuration.textureTypes & Specular) {
		glActiveTexture(GL_TEXTURE0 + textureCount);
		m_program.setUniform(m_uKs, textureCount);
		mat.getTexture(Specular)->bind();
		textureCount++;
	} else {
		m_program.setUniform(m_uKs, mat.getProperty<glm::vec3>(Ks));
	}

	if (textureCount > 0) {
		glActiveTexture(GL_TEXTURE0);
	}

	m_program.setUniform(m_uShininess, mat.getProperty<float>(Shininess));
}

void LightingShader::setUniforms(const std::vector<PointLight>& pLights) const {
	for (int i = 0; i < m_configuration.nPointLight; i++) {
		m_program.setUniform(m_uPointLights[i].ambient, pLights[i].ambient);
		m_program.setUniform(m_uPointLights[i].diffuse, pLights[i].diffuse);
		m_program.setUniform(m_uPointLights[i].specular, pLights[i].specular);

		m_program.setUniform(m_uPointLights[i].constant, pLights[i].constant);
		m_program.setUniform(m_uPointLights[i].linear, pLights[i].linear);
		m_program.setUniform(m_uPointLights[i].quadratic, pLights[i].quadratic);

		m_program.setUniform(m_uPointLights[i].position, pLights[i].position);
	}
}

void LightingShader::setUniforms(const std::vector<DirLight>& dLights) const {
	for (int i = 0; i < m_configuration.nDirLight; i++) {
		m_program.setUniform(m_uDirLights[i].ambient, dLights[i].ambient);
		m_program.setUniform(m_uDirLights[i].diffuse, dLights[i].diffuse);
		m_program.setUniform(m_uDirLights[i].specular, dLights[i].specular);

		m_program.setUniform(m_uDirLights[i].direction, dLights[i].direction);
	}
}

void LightingShader::setUniforms(const std::vector<SpotLight>& sLights) const {
	for (int i = 0; i < m_configuration.nSpotLight; i++) {
		m_program.setUniform(m_uSpotLights[i].ambient, sLights[i].ambient);
		m_program.setUniform(m_uSpotLights[i].diffuse, sLights[i].diffuse);
		m_program.setUniform(m_uSpotLights[i].specular, sLights[i].specular);

		m_program.setUniform(m_uSpotLights[i].constant, sLights[i].constant);
		m_program.setUniform(m_uSpotLights[i].linear, sLights[i].linear);
		m_program.setUniform(m_uSpotLights[i].quadratic, sLights[i].quadratic);

		m_program.setUniform(m_uSpotLights[i].position, sLights[i].position);
		m_program.setUniform(m_uSpotLights[i].direction, sLights[i].direction);

		m_program.setUniform(m_uSpotLights[i].cutOff, sLights[i].cutOff);
		m_program.setUniform(m_uSpotLights[i].outerCutOff, sLights[i].outerCutOff);
	}
}

void LightingShader::load() {
	m_vertex->setSourceFile("shaders/lighting.vert");
	m_fragment->setSourceFiles({ "shaders/lighting.frag", "shaders/" + m_configuration.name + ".frag" });
	
	//Defines
	std::vector<std::string> defines;
	//Lights
	if (m_configuration.nPointLight > 0) {
		defines.push_back("N_POINTLIGHT " + std::to_string(m_configuration.nPointLight));
	} else {
		defines.push_back("NO_POINTLIGHT");
	}
	if (m_configuration.nDirLight > 0) {
		defines.push_back("N_DIRLIGHT " + std::to_string(m_configuration.nDirLight));
	} else {
		defines.push_back("NO_DIRLIGHT");
	}
	if (m_configuration.nSpotLight > 0) {
		defines.push_back("N_SPOTLIGHT " + std::to_string(m_configuration.nSpotLight));
	} else {
		defines.push_back("NO_SPOTLIGHT");
	}

	//Textures
	if (m_configuration.textureTypes & Ambient)
		defines.push_back("AMBIENT_MAP");
	if (m_configuration.textureTypes & Diffuse)
		defines.push_back("DIFFUSE_MAP");
	if (m_configuration.textureTypes & Specular)
		defines.push_back("SPECULAR_MAP");

	m_fragment->setDefines(defines);
}

void LightingShader::fillUniformsLoc() {
	//Vertex
	m_uModel = glGetUniformLocation(m_program, "model");
	m_uNormalModel = glGetUniformLocation(m_program, "normalModel");
	m_uView = glGetUniformLocation(m_program, "view");
	m_uProjection = glGetUniformLocation(m_program, "projection");

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
		ulight.ambient = glGetUniformLocation(m_program, std::string("dirLights[" + n + "].ambient").c_str());
		ulight.diffuse = glGetUniformLocation(m_program, std::string("dirLights[" + n + "].diffuse").c_str());
		ulight.specular = glGetUniformLocation(m_program, std::string("dirLights[" + n + "].specular").c_str());
		ulight.direction = glGetUniformLocation(m_program, std::string("dirLights[" + n + "].direction").c_str());
		m_uDirLights.push_back(ulight);
	}
	m_uSpotLights.clear();
	for (int i = 0; i < m_configuration.nSpotLight; i++) {
		std::string n = std::to_string(i);
		uSpotLight ulight;
		ulight.position = glGetUniformLocation(m_program, std::string("spotLights[" + n + "].position").c_str());
		ulight.ambient = glGetUniformLocation(m_program, std::string("spotLights[" + n + "].ambient").c_str());
		ulight.diffuse = glGetUniformLocation(m_program, std::string("spotLights[" + n + "].diffuse").c_str());
		ulight.specular = glGetUniformLocation(m_program, std::string("spotLights[" + n + "].specular").c_str());
		ulight.constant = glGetUniformLocation(m_program, std::string("spotLights[" + n + "].constant").c_str());
		ulight.linear = glGetUniformLocation(m_program, std::string("spotLights[" + n + "].linear").c_str());
		ulight.quadratic = glGetUniformLocation(m_program, std::string("spotLights[" + n + "].quadratic").c_str());

		ulight.direction = glGetUniformLocation(m_program, std::string("spotLights[" + n + "].direction").c_str());
		ulight.cutOff = glGetUniformLocation(m_program, std::string("spotLights[" + n + "].cutOff").c_str());
		ulight.outerCutOff = glGetUniformLocation(m_program, std::string("spotLights[" + n + "].cutOff").c_str());
		m_uSpotLights.push_back(ulight);
	}
	m_uPointLights.clear();
	for (int i = 0; i < m_configuration.nPointLight; i++) {
		std::string n = std::to_string(i);
		uPointLight ulight;
		ulight.position = glGetUniformLocation(m_program, std::string("pointLights[" + n + "].position").c_str());
		ulight.ambient = glGetUniformLocation(m_program, std::string("pointLights[" + n + "].ambient").c_str());
		ulight.diffuse = glGetUniformLocation(m_program, std::string("pointLights[" + n + "].diffuse").c_str());
		ulight.specular = glGetUniformLocation(m_program, std::string("pointLights[" + n + "].specular").c_str());
		ulight.constant = glGetUniformLocation(m_program, std::string("pointLights[" + n + "].constant").c_str());
		ulight.linear = glGetUniformLocation(m_program, std::string("pointLights[" + n + "].linear").c_str());
		ulight.quadratic = glGetUniformLocation(m_program, std::string("pointLights[" + n + "].quadratic").c_str());
		m_uPointLights.push_back(ulight);
	}

	//Fragment : viewPos
	m_uViewPos = glGetUniformLocation(m_program, "viewPos");
}