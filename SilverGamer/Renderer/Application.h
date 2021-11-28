#ifndef SG_GRAPHICS_APPLICATION
#define SG_GRAPHICS_APPLICATION


#include "Graphics.h"

namespace Renderer
{

	class SGApplication
	{
	public:
		static SGApplication& GetInstance();
		void Init(int argc, char** argv);
		void Run();
		 
		SGGraphics* GetGraphics() { return m_graphicPipline; }
		GLFWwindow* GetGLFWWindow();

	protected:
		SGGraphics* m_graphicPipline;

	};


}


#endif