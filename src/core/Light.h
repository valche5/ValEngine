#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include "shader.h"

class Light {
public:
	//virtual void setUniforms(Shader &shader) = 0;
};

class PointLight : public Light {
public:
	PointLight(glm::vec3 position = glm::vec3(), glm::vec3 ambient = glm::vec3(0.05), glm::vec3 diffuse = glm::vec3(0.8)
		, glm::vec3 specular = glm::vec3(0.2), GLfloat constant = 1, GLfloat linear = 0.09, GLfloat quadratic = 0.032)
		: position(position), ambient(ambient), diffuse(diffuse), specular(specular), constant(constant), linear(linear)
		, quadratic(quadratic)
	{}

	virtual void setUniforms(Shader shader, size_t n) {
		std::string prefix = "pointLights[" + std::to_string(n) + "].";

		shader.setUniform(prefix + "position", position);
		shader.setUniform(prefix + "ambient", ambient);
		shader.setUniform(prefix + "diffuse", diffuse);
		shader.setUniform(prefix + "specular", specular);
		shader.setUniform(prefix + "constant", constant);
		shader.setUniform(prefix + "linear", linear);
		shader.setUniform(prefix + "quadratic", quadratic);

	}

	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	GLfloat constant;
	GLfloat linear;
	GLfloat quadratic;
};

class DirLight : public Light {
	DirLight(glm::vec3 direction = glm::vec3(0, 0, -1), glm::vec3 ambient = glm::vec3(0.05), glm::vec3 diffuse = glm::vec3(0.8)
		, glm::vec3 specular = glm::vec3(1))
		: direction(direction), ambient(ambient), diffuse(diffuse), specular(specular)
	{}

	virtual void setUniforms(Shader shader) {
		std::string prefix = "dirLight.";

		shader.setUniform(prefix + "direction", direction);
		shader.setUniform(prefix + "ambient", ambient);
		shader.setUniform(prefix + "diffuse", diffuse);
		shader.setUniform(prefix + "specular", specular);
	}

	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

class SpotLight : public Light {
	SpotLight(glm::vec3 position = glm::vec3(), glm::vec3 direction = glm::vec3(0, 0, -1), glm::vec3 ambient = glm::vec3(0.05)
		, glm::vec3 diffuse = glm::vec3(0.8), glm::vec3 specular = glm::vec3(1), GLfloat constant = 1, GLfloat linear = 0.09
		, GLfloat quadratic = 0.032, GLfloat cutOff = 0.97, GLfloat outerCutOff = 0.96)
		: position(position), direction(direction), ambient(ambient), diffuse(diffuse), specular(specular), constant(constant)
		, linear(linear), quadratic(quadratic), cutOff(cutOff), outerCutOff(outerCutOff)
	{}

	virtual void setUniforms(Shader shader, size_t n) {
		std::string prefix = "spotLight.";

		shader.setUniform(prefix + "position", position);
		shader.setUniform(prefix + "direction", direction);
		shader.setUniform(prefix + "ambient", ambient);
		shader.setUniform(prefix + "diffuse", diffuse);
		shader.setUniform(prefix + "specular", specular);
		shader.setUniform(prefix + "constant", constant);
		shader.setUniform(prefix + "linear", linear);
		shader.setUniform(prefix + "quadratic", quadratic);
		shader.setUniform(prefix + "cutOff", cutOff);
		shader.setUniform(prefix + "outerCutOff", position);
	}

	void setCutOff(GLfloat angle) {
		cutOff = glm::cos(glm::radians(angle));
	}

	void setOuterCutOff(GLfloat angle) {
		outerCutOff = glm::cos(glm::radians(angle));
	}

	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	GLfloat constant;
	GLfloat linear;
	GLfloat quadratic;
	GLfloat cutOff;
	GLfloat outerCutOff;
};

#endif // !LIGHT_H
