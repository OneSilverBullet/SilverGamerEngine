#ifndef SG_GRAPHICS_APPLICATION
#define SG_GRAPHICS_APPLICATION

#include "ApplicationConfig.h"

namespace SilverEngineLib
{
	class SGGeneralTimer;
}


struct ApplicationState
{
	int m_clientWidth = 1280;
	int m_clientHeigh = 720;
	bool m_applicationPaused = false;
	bool m_minimized = false;
	bool m_maximized = false;
	bool m_resizing = false;
	bool m_fullScreenState = false;
};

//Basic Application Class
class IApplication
{
public:
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	int Run();
	static IApplication* Instance();


protected:
	virtual void Update(const SilverEngineLib::SGGeneralTimer& timer) = 0;
	virtual void Render(const SilverEngineLib::SGGeneralTimer& timer) = 0;
	// Convenience overrides for handling mouse input.
	virtual void OnMouseDown(WPARAM btnState, int x, int y) { }
	virtual void OnMouseUp(WPARAM btnState, int x, int y) { }
	virtual void OnMouseMove(WPARAM btnState, int x, int y) { }


protected:
	bool InitMainWindow();
	bool InitGraphics(); 
	void FrameCalculate();
	void OnResize(); //The Size changed function

	static IApplication* m_instance;

protected:
	HINSTANCE m_appInstance;
	HWND m_mainWnd;

	ApplicationState m_state;
	SilverEngineLib::SGGeneralTimer m_timer;
	D3D12_VIEWPORT m_screenViewport;
	D3D12_RECT m_scissorRect;

	Device* m_device;
	SwapChain* m_swapChain;
	CommandQueue* m_commandQueue;
	CommandList* m_commandList;
	ApplicationConfig m_appConfig;
	Fence* m_fence;

};




#endif