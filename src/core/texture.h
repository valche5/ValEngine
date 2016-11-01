#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

#include "OpenGL.h"
#include "glassert.h"

class Texture
{
public:
    Texture();
	~Texture();
    void load(const std::string &path,
              GLenum min_filter = GL_LINEAR,
              GLenum mag_filter = GL_LINEAR,
              GLenum wrap_s = GL_REPEAT,
              GLenum wrap_t = GL_REPEAT);

	void bind();

    operator GLuint() { return texture; }

    GLuint texture;
    std::string type;
    std::string path;
};

#endif // TEXTURE_H
