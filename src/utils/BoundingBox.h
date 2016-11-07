#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <limits>
#include <algorithm>

#include <glm/glm.hpp>

#include <core/openGL.h>
#include <glw/VertexArray.h>

class AABB {
public:
	void fitWithAddingPoint(const glm::vec3 &point) {
		for (int i = 0; i < 3; i++) {
			min[i] = std::min(min[i], point[i]);
			max[i] = std::max(max[i], point[i]);
		}
	};

	void fitWithAddingBbox(const AABB &aabb) {
		for (int i = 0; i < 3; i++) {
			min[i] = std::min(min[i], aabb.min[i]);
			max[i] = std::max(max[i], aabb.max[i]);
		}
	};

	glm::vec3 getCenter() {
		return (max + min) / 2.f;
	}

	glm::vec3 getSize() {
		return glm::abs(max - min);
	}

	void initGL() {
		if (m_VAO != 0) return;

		GLfloat left = min.x;
		GLfloat right = max.x;

		std::vector<glm::vec3> vertices = {
			{ min.x, min.y, max.z },
			{ max.x, min.y, max.z },
			{ max.x, max.y, max.z },
			{ min.x, max.y, max.z },
			{ min.x, min.y, min.z },
			{ max.x, min.y, min.z },
			{ max.x, max.y, min.z },
			{ min.x, max.y, min.z },
		};

		std::vector<GLuint> indices = {
			0, 1,
			1, 2,
			2, 3,
			3, 0,
			4, 5,
			5, 6,
			6, 7,
			7, 4,
			0, 4,
			1, 5,
			2, 6,
			3, 7
		};

		gl::BufferPtr VBO(new gl::Buffer(gl::BufferTarget::ArrayBuffer));
		VBO->setData(vertices);
		VBO->setAttributes({ { 0, 3, GL_FLOAT, 0 } });
		gl::BufferPtr EBO(new gl::Buffer(gl::BufferTarget::ElementBuffer));

		m_VAO.bindVertexBuffers({ VBO, EBO });
	};

	void draw() {
		m_VAO.bind();
		glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
		m_VAO.unbind();
	};

private:
	gl::VertexArray m_VAO;

	glm::vec3 min = glm::vec3(std::numeric_limits<float>::infinity());
	glm::vec3 max = glm::vec3(-std::numeric_limits<float>::infinity());
};

#endif // BOUNDINGBOX_H
