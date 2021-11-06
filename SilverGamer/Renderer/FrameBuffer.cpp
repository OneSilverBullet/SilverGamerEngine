#include "FrameBuffer.h"
#include "RenderCheck.h"


Renderer::SGFrameBuffer::SGFrameBuffer(int texNum)
{
	int w = APPLICATION_WIDTH;
	int h = APLLICATION_HEIGHT;
	//创建深度缓冲区
	glGenRenderbuffers(1, &m_depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
	//创建fbo贴图
	for (int i = 0; i < texNum; ++i) {
		m_fboTextures.push_back(new SGTexture2D(w, h));
	}
	//创建framebuffer object
	glGenFramebuffers(1, &m_fbo);
	for (int i = 0; i < texNum; ++i) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_fboTextures[i]->GetTextureID(), 0);
	}
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);
	checkFrameBufferStatus();
	//清空Framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//绘制对应的buffer
void Renderer::SGFrameBuffer::drawBuffer(int buffer)
{
	glDrawBuffer(GL_COLOR_ATTACHMENT0 + buffer);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

//绘制多重buffer
void Renderer::SGFrameBuffer::drawBuffers(std::vector<int> buffers)
{
	const int n = buffers.size();
	//构造对应的绑定点
	std::vector<GLenum> attachments;
	for (int i = 0; i < n; ++i) {
		attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
	}
	glDrawBuffers(n, &attachments[0]); //绘制多重绑定点的buffer
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}
