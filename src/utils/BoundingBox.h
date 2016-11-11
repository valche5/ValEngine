#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <limits>
#include <algorithm>

#include <glm/glm.hpp>

class AABB {
public:
	glm::vec3 min = glm::vec3(std::numeric_limits<float>::infinity());
	glm::vec3 max = glm::vec3(-std::numeric_limits<float>::infinity());

public:
	glm::vec3 getCenter() const {
		return (min + max) / 2.f;
	}

	glm::vec3 getSize() const {
		return (max - min);
	}

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
};

#endif // BOUNDINGBOX_H
