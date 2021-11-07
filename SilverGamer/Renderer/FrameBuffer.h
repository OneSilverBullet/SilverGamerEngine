#ifndef SG_FRAME_BUFFER
#define SG_FRAME_BUFFER

#include "RenderSetting.h"
#include "Texture2D.h"

namespace Renderer
{



	//TODO: bind to GPU output
	class SGFrameBuffer
	{
	public:

		SGFrameBuffer(int texNum);
		void Enable() { glBindFramebuffer(GL_FRAMEBUFFER, m_fbo); }
		void Disable() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
		void DrawBuffer(int buffer);
		void DrawBuffers(); //激活当前所有的framebuffer.
		std::vector<Renderer::SGTexture2D*> GetFBOTextures() { return m_fboTextures; }

	private:
		GLuint m_fbo;
		GLuint m_depthBuffer;
		std::vector<Renderer::SGTexture2D*> m_fboTextures;
	};

}
#endif