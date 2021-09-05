#ifndef SG_TEXTURE_2D
#define SG_TEXTURE_2D
#include "RenderSetting.h"


namespace Renderer
{

	class SGTexture2D
	{
	public:
		GLuint m_id;
		std::string m_name;
		SGTexture2D(const std::string name, const std::string path);
		SGTexture2D(const std::string name, int w, int h, GLenum magFilter = GL_NEAREST, GLenum minFilter = GL_NEAREST,
			GLuint internalFormat = GL_RGB16F, GLint type = GL_FLOAT, GLint wrap= GL_REPEAT);
		SGTexture2D();

		void activate(GLuint program, GLuint textureLoc);

		static SGTexture2D* Default() {
			return new SGTexture2D("tex", 64, 64);
		}

		static SGTexture2D* Texture2DDefault() {
			return new SGTexture2D("miniTex", 64, 64);
		}

	private:
		int width, height;
	};
}

#endif