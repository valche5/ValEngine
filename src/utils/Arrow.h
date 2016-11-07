#ifndef ARROW_H
#define ARROW_H

#include <vector>

#include <glm/glm.hpp>

#include <core/openGL.h>
#include <glw/VertexArray.h>

class Arrow {
public:
	Arrow() {
		n = 20;
		std::vector<glm::vec3> completeCone = getCone(glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), 0.1, 0.04, n);
		completeCone.insert(completeCone.begin(), { glm::vec3(0, 0, 0), glm::vec3(0, 0, 0.9) });

		gl::BufferPtr VBO(new gl::Buffer(gl::BufferTarget::ArrayBuffer));
		VBO->setData(completeCone);
		VBO->setAttributes({ { 0, 3, GL_FLOAT, 0 } });

		m_VAO.bindVertexBuffer(VBO);
	}
	~Arrow() {
	}

	void draw() {
		m_VAO.bind();
		glDrawArrays(GL_LINES, 0, 2);
		glDrawArrays(GL_TRIANGLE_FAN, 2, n + 2);
		glDrawArrays(GL_TRIANGLE_FAN, n + 4, n + 2);
		m_VAO.unbind();
	}

	glm::vec3 perp(const glm::vec3 &v) {
		float min = fabs(v.x);
		glm::vec3 cardinalAxis(1, 0, 0);

		if (fabs(v.y) < min) {
			min = fabs(v.y);
			cardinalAxis = glm::vec3(0, 1, 0);
		}

		if (fabs(v.z) < min) {
			cardinalAxis = glm::vec3(0, 0, 1);
		}

		return glm::cross(v, cardinalAxis);
	}

	//d : Axis defined as a normalized vector from base to apex.
	//a : Position of apex.
	//h : Height of cone.
	//rd : Radius of directrix
	//n : Number of radial "slices."
	//Return : [0, n] : Top,  [n + 1, 2n + 1] -> bottom
	//Draw with triangle fan
	std::vector<glm::vec3> getCone(const glm::vec3 &d, const glm::vec3 &a,
		const float h, const float rd, const int n) {
		glm::vec3 c = a + (-d * h);
		glm::vec3 e0 = perp(d);
		glm::vec3 e1 = glm::cross(e0, d);
		float angInc = 360.0 / n * glm::pi<float>() / 180;

		// calculate points around directrix
		std::vector<glm::vec3> pts;
		for (int i = 0; i < n; ++i) {
			float rad = angInc * i;
			glm::vec3 p = c + (((e0 * cos(rad)) + (e1 * sin(rad))) * rd);
			pts.push_back(p);
		}

		std::vector<glm::vec3> result;
		// draw cone top
		result.push_back(a);
		for (int i = 0; i < n; ++i) {
			result.push_back(pts[i]);
		}
		result.push_back(pts[0]);

		// draw cone bottom
		result.push_back(c);
		for (int i = n - 1; i >= 0; --i) {
			result.push_back(pts[i]);
		}
		result.push_back(pts[0]);

		return result;
	}
private:
	int n;
	gl::VertexArray m_VAO;
};

#endif // ARROW_H
