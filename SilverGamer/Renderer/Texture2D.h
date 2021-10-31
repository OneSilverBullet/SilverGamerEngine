#ifndef SG_TEXTURE_2D
#define SG_TEXTURE_2D
#include "RenderSetting.h"


namespace Renderer
{

	class SGTexture2D
	{
	public:
		SGTexture2D();
		SGTexture2D(SG_TEXTURE_TYPE type, int width, int height, GLuint textureId);
		//For Custom Texture
		SGTexture2D(int w, int h, 
			GLenum magFilter = GL_NEAREST, GLenum minFilter = GL_NEAREST,
			GLuint internalFormat = GL_RGB16F, GLint type = GL_FLOAT, GLint wrap= GL_REPEAT);
		//UploadTexture
		void Upload(GLuint program, int location); 
		
	private:
		int m_width, m_height; 
		GLuint m_textureId;
		SG_TEXTURE_TYPE m_type;
	};
}

#endif