#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <iostream>

namespace gl {

Texture::Texture() {
	glGenTextures(1, &m_id);
}

Texture::~Texture() {
	glDeleteTextures(1, &m_id);
}

void Texture::load(const std::string &path, GLenum min_filter, GLenum mag_filter, GLenum wrap_s, GLenum wrap_t) {
	glBindTexture(GL_TEXTURE_2D, m_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);	// Set texture wrapping to GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);

	int width, height, perPixComp;
	const char *image_path = path.c_str();
	unsigned char* image = stbi_load(image_path, &width, &height, &perPixComp, 3);
	if (!image) {
		std::cout << "ERROR::TEXTURE::LOADING_FAILED\n" << stbi_failure_reason() << std::endl;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(image);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind() {
	glBindTexture(GL_TEXTURE_2D, m_id);
}

}