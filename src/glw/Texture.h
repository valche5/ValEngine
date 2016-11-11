#ifndef TEXTUREGL_H
#define TEXTUREGL_H

#include <string>
#include <memory>

#include <glw/GLObject.h>

namespace gl {

class Texture : public GLObject {
public:
	std::string name;
public:
	Texture();
	~Texture();
	void load(unsigned char *image,
			  int width,
			  int height,
			  GLenum min_filter = GL_LINEAR,
			  GLenum mag_filter = GL_LINEAR,
			  GLenum wrap_s = GL_REPEAT,
			  GLenum wrap_t = GL_REPEAT);
	bool load(const std::string &path,
			  GLenum min_filter = GL_LINEAR,
			  GLenum mag_filter = GL_LINEAR,
			  GLenum wrap_s = GL_REPEAT,
			  GLenum wrap_t = GL_REPEAT);

	void bind();

private:
	std::string m_path;
};

}

#endif // TEXTUREGL_H
