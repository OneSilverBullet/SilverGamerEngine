#include "RenderCheck.h"

//检查当前FBO状态是否正确
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
