#ifndef SG_GRAPHICS_APPLICATION
#define SG_GRAPHICS_APPLICATION

#include "RenderSetting.h"
#include "Graphics.h"
#include "Controller.h"

namespace Renderer
{

	class SGApplication
	{
	public:
		static SGApplication& GetInstance();
		void Init(int argc, char** argv);
		void Run();
		
		SGGraphics* GetGraphics() { return m_graphicPipline; }
		SGControllerFPS* GetController() { return m_controller; }

	protected:
		SGGraphics* m_graphicPipline;
		SGControllerFPS* m_controller;
		int window;



	};


}


#endif