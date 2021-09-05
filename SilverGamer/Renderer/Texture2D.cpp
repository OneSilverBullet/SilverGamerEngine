#include "Texture2D.h"
#include "stb_image.h"
#include "ResourceLoad.h"

Renderer::SGTexture2D::SGTexture2D(const std::string name, const std::string path)
{
	m_name = name;
	m_id = ResourceLoad::GetInstance()->LoadTexture2DResource(name, path);

	//获取当前Texture的height和width
	glBindTexture(GL_TEXTURE_2D, m_id);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Renderer::SGTexture2D::SGTexture2D(const std::string name, int w, int h, GLenum magFilter, GLenum minFilter,
	GLuint internalFormat, GLint type, GLint wrap)
{
	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	//构建空Texture贴图
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, GL_RGBA, type, 0);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	width = w;
	height = h;
	m_name = name;
}

Renderer::SGTexture2D::SGTexture2D()
{
}

//绑定对应shader程序，以及对应槽位
void Renderer::SGTexture2D::activate(GLuint program, GLuint textureLoc)
{
	glActiveTexture(GL_TEXTURE0 + textureLoc);
	glBindTexture(GL_TEXTURE_2D, m_id);
	//绑定到对应位置
	int tex_loc = glGetUniformLocation(program, m_name.c_str());
	if (tex_loc != -1) {
		glUniform1i(tex_loc, textureLoc);
	}
}
