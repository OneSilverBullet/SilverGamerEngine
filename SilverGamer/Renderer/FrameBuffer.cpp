#include "FrameBuffer.h"
#include "RenderCheck.h"

/*
  

*/

Renderer::SGFrameBuffer::SGFrameBuffer(int texNum)
{
	int w = APPLICATION_WIDTH;
	int h = APLLICATION_HEIGHT;
	//����framebuffer object
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

		//����fbo��ͼ
	for (int i = 0; i < texNum; ++i) {
		m_fboTextures.push_back(new SGTexture2D(w, h));
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_fboTextures[i]->GetTextureID(), 0);
	}

	const int n = m_fboTextures.size();
	//�����Ӧ�İ󶨵�
	std::vector<GLenum> attachments;
	for (int i = 0; i < n; ++i) {
		attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
	}
	glDrawBuffers(n, &attachments[0]); //���ƶ��ذ󶨵��buffer
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	//������Ȼ�����
	glGenRenderbuffers(1, &m_depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);
	if (!checkFrameBufferStatus())
	{
		std::cout << "Frame Buffer Error: Initialize Failed." << std::endl;
	}
	//���Framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//���ƶ�Ӧ��buffer
void Renderer::SGFrameBuffer::DrawBuffer(int buffer)
{
	glDrawBuffer(GL_COLOR_ATTACHMENT0 + buffer);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

//���ƶ���buffer
void Renderer::SGFrameBuffer::DrawBuffers()
{
	const int n = m_fboTextures.size();
	//�����Ӧ�İ󶨵�
	std::vector<GLenum> attachments;
	for (int i = 0; i < n; ++i) {
		attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
	}
	glDrawBuffers(n, &attachments[0]); //���ƶ��ذ󶨵��buffer
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}
