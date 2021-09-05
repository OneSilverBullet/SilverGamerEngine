#ifndef SG_FRAME_BUFFER
#define SG_FRAME_BUFFER

#include "RenderSetting.h"
#include "Texture2D.h"

namespace Renderer
{
	class SGFrameBuffer
	{
	public:
		GLuint m_fbo;
		GLuint m_depthBuffer;
		GLuint m_stencilBuffer;
		std::vector<Renderer::SGTexture2D*> m_fboTextures;

		SGFrameBuffer(int texNum);
		void enable() { glBindFramebuffer(GL_FRAMEBUFFER, m_fbo); }
		void disable() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
		void drawBuffer(int buffer);
		void drawBuffers(std::vector<int> buffers);

	};

}
#endif