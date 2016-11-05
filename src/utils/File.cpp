#include "File.h"

namespace util {
	std::string readFile(const std::string &path) {
		std::string content;
		std::ifstream file;

		file.exceptions(std::ifstream::badbit);
		try {
			file.open(path);

			std::stringstream fileStream;
			fileStream << file.rdbuf();

			file.close();

			content = fileStream.str();
		} catch (std::ifstream::failure e) {
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULY_READ" << std::endl;
		}

		return content;
	}
}