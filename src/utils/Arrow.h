#ifndef ARROW_H
#define ARROW_H

#include <vector>

#include "../core/openGL.h"
#include <glm/glm.hpp>

class Arrow {
public:
	Arrow() {
		GLfloat lineVertices[] = {
			0.f, 0.f, 0.9f
		};

		glGenVertexArrays(1, &coneVAO);
		glGenVertexArrays(1, &bottomVAO);
		glGenVertexArrays(1, &lineVAO);
		glGenBuffers(1, &coneVBO);
		glGenBuffers(1, &bottomVBO);
		glGenBuffers(1, &lineVBO);

		n = 20;
		std::vector<glm::vec3> completeCone = getCone(glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), 0.1, 0.04, n);
		std::vector<glm::vec3> cone(completeCone.begin(), completeCone.begin() + n + 3);
		std::vector<glm::vec3> bottom(completeCone.begin() + n + 3, completeCone.end());

		glBindVertexArray(coneVAO);
		glBindBuffer(GL_ARRAY_BUFFER, coneVBO);
		glBufferData(GL_ARRAY_BUFFER, (n + 2) * sizeof(glm::vec3), &cone[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		glBindVertexArray(bottomVAO);
		glBindBuffer(GL_ARRAY_BUFFER, bottomVBO);
		glBufferData(GL_ARRAY_BUFFER, (n + 2) * sizeof(glm::vec3), &bottom[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		glBindVertexArray(lineVAO);
		glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	void draw() {
		glBindVertexArray(coneVAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, n+1);
		glBindVertexArray(bottomVAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, n+1);
		glBindVertexArray(lineVAO);
		glDrawArrays(GL_LINES, 0, 2);
		glBindVertexArray(0);
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
		result.push_back(a);

		// draw cone bottom
		result.push_back(c);
		for (int i = n - 1; i >= 0; --i) {
			result.push_back(pts[i]);
		}
		result.push_back(c);

		return result;
	}
private:
	GLuint coneVAO, coneVBO, bottomVAO, bottomVBO, lineVAO, lineVBO;
	int n;
};

#endif // ARROW_H
