#include "Texture3D.h"


Renderer::Texture3D::Texture3D(const std::string name, int size, GLenum magFilter, GLenum minFilter, GLint internalFormat, GLint type, GLint wrap)
{
	glGenTextures(1, &m_id); 
	glBindTexture(GL_TEXTURE_3D, m_id);

	glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, wrap);
	glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, wrap);
	glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, magFilter);

	glTexStorage3D(GL_TEXTURE_3D, 7, internalFormat, size, size, size);
	glTexImage3D(GL_TEXTURE_3D, 0, internalFormat, size, size, size, 0, GL_RGBA, type, 0);

	glGenerateMipmap(GL_TEXTURE_3D);
	width = size;
	height = size;
	depth = size;
	m_name = name;
}

//¼¤»îMap
void Renderer::Texture3D::ActivateMap(GLuint program, GLuint textureLoc)
{
	glActiveTexture(GL_TEXTURE0 + textureLoc);
	glBindTexture(GL_TEXTURE_3D, m_id);
	int tex_loc = glGetUniformLocation(program, m_name.c_str());
	if (tex_loc != -1)
	{
		glUniform1i(tex_loc, textureLoc);
	}
}

//Çå³ýÑÕÉ«
void Renderer::Texture3D::Clear(glm::vec4 color)
{
	//glBindTexture(GL_TEXTURE_3D, m_id);
	//glClearTexImage(m_id, 0, GL_RGBA, GL_FLOAT, &color);
	//glBindTexture(GL_TEXTURE_3D, 0);
}
