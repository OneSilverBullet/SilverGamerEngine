#include "RenderCheck.h"

//��鵱ǰFBO״̬�Ƿ���ȷ
bool Renderer::checkFrameBufferStatus()
{
	GLenum status;
	status = (GLenum)glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch (status)
	{
	case GL_FRAMEBUFFER_COMPLETE:
		return true;

	default:
		break;
	}
	return false;
}
