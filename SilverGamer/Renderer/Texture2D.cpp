#include "Texture2D.h"
#include "stb_image.h"


Renderer::SGTexture2D::SGTexture2D()
{
}

Renderer::SGTexture2D::SGTexture2D(SG_TEXTURE_TYPE type, int width, int height, GLuint textureId):
	m_width(width), m_height(height), m_type(type), m_textureId(textureId)
{
}

Renderer::SGTexture2D::SGTexture2D(int w, int h, GLenum magFilter, GLenum minFilter, GLuint internalFormat, GLint type, GLint wrap)
{
	//This Initializer is for custom texture
	m_type = SG_TEXTURE_TYPE::TEXTURE_CUSTOM;
	glGenTextures(1, &m_textureId);
	glBindTexture(GL_TEXTURE_2D, m_textureId);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	//构建空Texture贴图
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, GL_RGBA, type, 0);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	m_width = w;
	m_height = h;
}

/*
* program: shader program
* location: texture location
*/
void Renderer::SGTexture2D::Upload(GLuint program, int location)
{
	glActiveTexture(GL_TEXTURE0 + location);
	glBindTexture(GL_TEXTURE_2D, m_textureId);
	//绑定到对应位置
	int tex_loc = glGetUniformLocation(program, SG_TEXTURE_GPU_HOOK[m_type].c_str());
	if (tex_loc != -1) {
		glUniform1i(tex_loc, location);
	}
	else
	{
		std::cout << "Texture Error: Texture Load Failed." << std::endl;
	}
}

void Renderer::SGTexture2D::Upload(GLuint program, std::string gpuHook, int location)
{
	glActiveTexture(GL_TEXTURE0 + location);
	glBindTexture(GL_TEXTURE_2D, m_textureId);
	//绑定到对应位置
	int tex_loc = glGetUniformLocation(program, gpuHook.c_str());
	if (tex_loc != -1) {
		glUniform1i(tex_loc, location);
	}
	else
	{
		std::cout << "Texture Error: Texture Load Failed." << std::endl;
	}
}

