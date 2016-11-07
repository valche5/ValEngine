#ifndef TYPEGL_H
#define TYPEGL_H

#include <memory>
#include <vector>

namespace gl {

class Shader;
class ShaderProgram;
class Texture;
class Buffer;

typedef std::shared_ptr<Shader> ShaderPtr;
typedef std::shared_ptr<ShaderProgram> ShaderProgramPtr;
typedef std::shared_ptr<Texture> TexturePtr;
typedef std::shared_ptr<Buffer> BufferPtr;

}



#endif // TYPEGL_H
