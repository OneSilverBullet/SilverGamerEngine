#ifndef SG_GRAPHICS_APPLICATION
#define SG_GRAPHICS_APPLICATION

#include "ApplicationConfig.h"

//Basic Application Class
class IApplication
{
public:
	bool InitMainWindow(); 




private:
	HINSTANCE m_appInstance;
	HWND m_mainWnd;

	Device* m_device;
	SwapChain* m_swapChain;
	CommandQueue* m_commandQueue;
	CommandList* m_commandList;
	ApplicationConfig m_appConfig;


};




#endif