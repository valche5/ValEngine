#include "glmstreams.h"

std::ostream & operator<<(std::ostream & stream, const glm::vec3 & vec3) {
	stream << "(" << vec3.x << "," << vec3.y << "," << ")";
	return stream;
}
