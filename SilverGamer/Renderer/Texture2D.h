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
		void Upload(GLuint program, std::string gpuHook, int location);
		GLuint GetTextureID() { return m_textureId; }

	private:
		int m_width, m_height; 
		GLuint m_textureId;
		SG_TEXTURE_TYPE m_type;

		std::map<int, std::string> SG_TEXTURE_GPU_HOOK = {
			{1, "material.normal"},
			{2, "material.diffuse"},
			{6, "material.emit"},
			{4, "material.roughness"},
			{5, "material.metallic"},
			{3, "material.ao"}
		};
	};
}

#endif