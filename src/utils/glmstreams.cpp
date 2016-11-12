#include "glmstreams.h"

#include <string> 

std::ostream & operator<<(std::ostream & stream, const glm::vec3 & vec3) {
	stream << "(" << vec3.x << "," << vec3.y << "," << vec3.z << ")";
	return stream;
}

std::string toString(const glm::vec3 & vec3) {
	return "(" + std::to_string(vec3.x) + "," + std::to_string(vec3.y) + "," + std::to_string(vec3.z) + ")";;
}
