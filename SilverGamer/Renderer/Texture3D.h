#ifndef SG_TEXTURE_2D
#define SG_TEXTURE_2D
#include "RenderSetting.h"

namespace Renderer
{
	class Texture3D
	{
	public:
		GLuint m_id;
		std::string m_name;
		Texture3D(const std::string name, int size, GLenum magFilter = GL_NEAREST, GLenum minFilter = GL_NEAREST,
			GLint internalFormat = GL_RGBA, GLint type = GL_FLOAT, GLint wrap = GL_REPEAT);
		Texture3D() {}

		void ActivateMap(GLuint program, GLuint textureLoc);
		void Clear(glm::vec4 color = glm::vec4(0));

	private:
		int width, height, depth;
	};
}




#endif
