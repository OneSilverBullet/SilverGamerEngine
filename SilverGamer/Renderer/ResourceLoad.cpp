#include "ResourceLoad.h"

Renderer::ResourceLoad* Renderer::ResourceLoad::m_instance = nullptr;

int Renderer::ResourceLoad::LoadTexture2DResource(std::string name, std::string path)
{
	unsigned int texture;
	glGenTextures(1, &texture);

	// 加载并生成纹理
	int width, height, nrChannels;
	unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, texture);


		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// 为当前绑定的纹理对象设置环绕、过滤方式


		stbi_image_free(data);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		return -1;
	}
	m_textures[name] = texture;

	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}
