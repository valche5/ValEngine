#include "MeshShader.h"

#include <core/Scene.h>


void FillShader::setUniforms(const Camera & camera) const {
	m_program.setUniform(m_uView, camera.getViewMatrix());
	m_program.setUniform(m_uProjection, camera.getProjection());
}

void FillShader::setUniforms(const glm::mat4 & model) const {
	m_program.setUniform(m_uModel, model);
}

void FillShader::setUniforms(const Material & mat) const {
	m_program.setUniform(m_uFillColor, mat.getProperty<glm::vec3>(MaterialProperty::FillColor));
}

void FillShader::setUniforms(const std::vector<PointLight>& pLights) const {
}

void FillShader::setUniforms(const std::vector<DirLight>& dLights) const {
}

void FillShader::setUniforms(const std::vector<SpotLight>& sLights) const {
}

void FillShader::loadShaders(gl::ShaderPtr vertex, gl::ShaderPtr fragment, const ShaderConfiguration &configuration) {
	vertex->setSourceFile("shaders/fill.vert");
	fragment->setSourceFile("shaders/fill.frag");
}

void FillShader::fillUniformsLoc(gl::ShaderProgram &program, const ShaderConfiguration &configuration) {
	m_uView = program.getUniformLoc("view");
	m_uProjection = program.getUniformLoc("projection");
	m_uModel = program.getUniformLoc("model");
	m_uFillColor = program.getUniformLoc("fillColor");
}

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
	if (mat.textureTypes & Ambient) {
		glActiveTexture(GL_TEXTURE0 + textureCount);
		m_program.setUniform(m_uKa, textureCount);
		mat.getTexture(Ambient)->bind();
		textureCount++;
	} else {
		m_program.setUniform(m_uKa, mat.getProperty<glm::vec3>(Ka));
	}

	if (mat.textureTypes & Diffuse) {
		glActiveTexture(GL_TEXTURE0 + textureCount);
		m_program.setUniform(m_uKd, textureCount);
		mat.getTexture(Diffuse)->bind();
		textureCount++;
	} else {
		m_program.setUniform(m_uKd, mat.getProperty<glm::vec3>(Kd));
	}

	if (mat.textureTypes & Specular) {
		glActiveTexture(GL_TEXTURE0 + textureCount);
		m_program.setUniform(m_uKs, textureCount);
		mat.getTexture(Specular)->bind();
		textureCount++;
	} else {
		m_program.setUniform(m_uKs, mat.getProperty<glm::vec3>(Ks));
	}

	if (mat.textureTypes & Normal) {
		glActiveTexture(GL_TEXTURE0 + textureCount);
		m_program.setUniform(m_uNormalMap, textureCount);
		mat.getTexture(Normal)->bind();
		textureCount++;
	}

	if (textureCount > 0) {
		glActiveTexture(GL_TEXTURE0);
	}

	m_program.setUniform(m_uShininess, mat.getProperty<float>(Shininess));
}

void LightingShader::setUniforms(const std::vector<PointLight>& pLights) const {
	for (int i = 0; i < pLights.size(); i++) {
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
	for (int i = 0; i < dLights.size(); i++) {
		m_program.setUniform(m_uDirLights[i].ambient, dLights[i].ambient);
		m_program.setUniform(m_uDirLights[i].diffuse, dLights[i].diffuse);
		m_program.setUniform(m_uDirLights[i].specular, dLights[i].specular);

		m_program.setUniform(m_uDirLights[i].direction, dLights[i].direction);
	}
}

void LightingShader::setUniforms(const std::vector<SpotLight>& sLights) const {
	for (int i = 0; i < sLights.size(); i++) {
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

void LightingShader::loadShaders(gl::ShaderPtr vertex, gl::ShaderPtr fragment, const ShaderConfiguration &configuration) {
	vertex->setSourceFile("shaders/lighting.vert");
	fragment->setSourceFiles({ "shaders/lighting.frag", "shaders/" + configuration.lightingFct + ".frag" });
	
	//Defines
	std::vector<std::string> defines;
	//Lights
	if (m_nPointLight > 0) {
		defines.push_back("N_POINTLIGHT " + std::to_string(m_nPointLight));
	} else {
		defines.push_back("NO_POINTLIGHT");
	}
	if (m_nDirLight > 0) {
		defines.push_back("N_DIRLIGHT " + std::to_string(m_nDirLight));
	} else {
		defines.push_back("NO_DIRLIGHT");
	}
	if (m_nSpotLight > 0) {
		defines.push_back("N_SPOTLIGHT " + std::to_string(m_nSpotLight));
	} else {
		defines.push_back("NO_SPOTLIGHT");
	}

	//Textures
	if (configuration.textureTypes & Ambient)
		defines.push_back("AMBIENT_MAP");
	if (configuration.textureTypes & Diffuse)
		defines.push_back("DIFFUSE_MAP");
	if (configuration.textureTypes & Specular)
		defines.push_back("SPECULAR_MAP");
	if (configuration.textureTypes & Normal)
		defines.push_back("NORMAL_MAP");

	fragment->setDefines(defines);
}

void LightingShader::fillUniformsLoc(gl::ShaderProgram &program, const ShaderConfiguration &configuration) {
	//Vertex
	m_uModel = program.getUniformLoc("model");
	m_uNormalModel = program.getUniformLoc("normalModel");
	m_uView = program.getUniformLoc("view");
	m_uProjection = program.getUniformLoc("projection");

	//Fragment : Material
	if (configuration.textureTypes & Ambient)
		m_uKa = program.getUniformLoc("ambient");
	else
		m_uKa = program.getUniformLoc("ka");

	if (configuration.textureTypes & Diffuse)
		m_uKd = program.getUniformLoc("diffuse");
	else
		m_uKd = program.getUniformLoc("kd");

	if (configuration.textureTypes & Specular)
		m_uKs = program.getUniformLoc("specular");
	else
		m_uKs = program.getUniformLoc("ks");
	if (configuration.textureTypes & Normal)
		m_uNormalMap = program.getUniformLoc("normalMap");

	m_uShininess = program.getUniformLoc("shininess");

	//Fragment : Lights
	m_uDirLights.clear();
	for (int i = 0; i < m_nDirLight; i++) {
		std::string n = std::to_string(i);
		uDirLight ulight;
		ulight.ambient = program.getUniformLoc(std::string("dirLights[" + n + "].ambient").c_str());
		ulight.diffuse = program.getUniformLoc(std::string("dirLights[" + n + "].diffuse").c_str());
		ulight.specular = program.getUniformLoc(std::string("dirLights[" + n + "].specular").c_str());
		ulight.direction = program.getUniformLoc(std::string("dirLights[" + n + "].direction").c_str());
		m_uDirLights.push_back(ulight);
	}
	m_uSpotLights.clear();
	for (int i = 0; i < m_nSpotLight; i++) {
		std::string n = std::to_string(i);
		uSpotLight ulight;
		ulight.position = program.getUniformLoc(std::string("spotLights[" + n + "].position").c_str());
		ulight.ambient = program.getUniformLoc(std::string("spotLights[" + n + "].ambient").c_str());
		ulight.diffuse = program.getUniformLoc(std::string("spotLights[" + n + "].diffuse").c_str());
		ulight.specular = program.getUniformLoc(std::string("spotLights[" + n + "].specular").c_str());
		ulight.constant = program.getUniformLoc(std::string("spotLights[" + n + "].constant").c_str());
		ulight.linear = program.getUniformLoc(std::string("spotLights[" + n + "].linear").c_str());
		ulight.quadratic = program.getUniformLoc(std::string("spotLights[" + n + "].quadratic").c_str());

		ulight.direction = program.getUniformLoc(std::string("spotLights[" + n + "].direction").c_str());
		ulight.cutOff = program.getUniformLoc(std::string("spotLights[" + n + "].cutOff").c_str());
		ulight.outerCutOff = program.getUniformLoc(std::string("spotLights[" + n + "].cutOff").c_str());
		m_uSpotLights.push_back(ulight);
	}
	m_uPointLights.clear();
	for (int i = 0; i < m_nPointLight; i++) {
		std::string n = std::to_string(i);
		uPointLight ulight;
		ulight.position = program.getUniformLoc(std::string("pointLights[" + n + "].position").c_str());
		ulight.ambient = program.getUniformLoc(std::string("pointLights[" + n + "].ambient").c_str());
		ulight.diffuse = program.getUniformLoc(std::string("pointLights[" + n + "].diffuse").c_str());
		ulight.specular = program.getUniformLoc(std::string("pointLights[" + n + "].specular").c_str());
		ulight.constant = program.getUniformLoc(std::string("pointLights[" + n + "].constant").c_str());
		ulight.linear = program.getUniformLoc(std::string("pointLights[" + n + "].linear").c_str());
		ulight.quadratic = program.getUniformLoc(std::string("pointLights[" + n + "].quadratic").c_str());
		m_uPointLights.push_back(ulight);
	}

	//Fragment : viewPos
	m_uViewPos = program.getUniformLoc("viewPos");
}
