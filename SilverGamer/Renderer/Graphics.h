#ifndef SG_GRAPHICS
#define SG_GRAPHICS

#include "RenderSetting.h"
#include "Scene.h"
#include "FrameBuffer.h"

namespace Renderer
{

	class SGGraphics
	{
	public:
		SGGraphics();
		~SGGraphics();

		virtual void Init();
		virtual void Render();

		GLFWwindow* GetGLFWWindow() { return m_window; }
		SGScene* GetScene() { return m_scene; }
		SGFrameBuffer* GetFrameBuffer() { return m_frameBuffer; }

	private:
		std::unordered_map<std::string, GLuint> m_shaderInstanceMap;
		GLuint m_shaderInstance;


		GLFWwindow* m_window;
		SGFrameBuffer* m_frameBuffer; 
		SGScene* m_scene;
		int w, h;
	};



}



#endif