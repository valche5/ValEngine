#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

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

	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	GLfloat constant;
	GLfloat linear;
	GLfloat quadratic;
};

class DirLight : public Light {
public:
	DirLight(glm::vec3 direction = glm::vec3(0, 0, -1), glm::vec3 ambient = glm::vec3(0.05), glm::vec3 diffuse = glm::vec3(0.8)
		, glm::vec3 specular = glm::vec3(1))
		: direction(direction), ambient(ambient), diffuse(diffuse), specular(specular)
	{}

	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

class SpotLight : public Light {
public:
	SpotLight(glm::vec3 position = glm::vec3(), glm::vec3 direction = glm::vec3(0, 0, -1), glm::vec3 ambient = glm::vec3(0.05)
		, glm::vec3 diffuse = glm::vec3(0.8), glm::vec3 specular = glm::vec3(1), GLfloat constant = 1, GLfloat linear = 0.09
		, GLfloat quadratic = 0.032, GLfloat cutOff = 0.97, GLfloat outerCutOff = 0.96)
		: position(position), direction(direction), ambient(ambient), diffuse(diffuse), specular(specular), constant(constant)
		, linear(linear), quadratic(quadratic), cutOff(cutOff), outerCutOff(outerCutOff)
	{}

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
