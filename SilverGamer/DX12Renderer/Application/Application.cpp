#include "Application.h"
#include <windowsx.h>

LRESULT CALLBACK
MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Forward hwnd on because we can get messages (e.g., WM_CREATE)
	// before CreateWindow returns, and thus before mhMainWnd is valid.
	return  IApplication::Instance()->MsgProc(hwnd, msg, wParam, lParam);
}

/*
* IApplication
*/
bool IApplication::InitMainWindow()
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_appInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"SilverGamer";
	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass Failed.", 0, 0);
		return false;
	}

	// Compute window rectangle dimensions based on requested client area dimensions.
	RECT R = { 0, 0, m_appConfig.GetWidth(), m_appConfig.GetHeight() };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int width = R.right - R.left;
	int height = R.bottom - R.top;

	m_mainWnd = CreateWindow(L"MainWnd", m_appConfig.GetMainWndCaption().c_str(),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		width, height, 0, 0, m_appInstance, 0);
	if (!m_mainWnd)
	{
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}

	ShowWindow(m_mainWnd, SW_SHOW);
	UpdateWindow(m_mainWnd);
	return true;
}

bool IApplication::InitGraphics()
{
	m_device = new Device(D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0);
	m_device->CreateFence(m_fence);
	m_commandQueue = new CommandQueue(m_device);
	m_commandList = new CommandList(m_device);
	m_swapChain = new SwapChain(m_commandQueue, m_appConfig, m_mainWnd);
	return false;
}

//Frame Calculate
void IApplication::FrameCalculate()
{
	static int frameCounter = 0;
	static float timeElapsed = 0.0f;
	frameCounter++;
	if (m_timer.TotalTime() - timeElapsed >= 1.0f)
	{
		float fps = (float)frameCounter;
		float mspf = 1000.0f / fps; //ms number every fps

		std::wstring fpsStr = std::to_wstring(fps);
		std::wstring mspfStr = std::to_wstring(mspf);
		std::wstring windowText = m_appConfig.GetMainWndCaption() +
			L"    fps: " + fpsStr +
			L"   mspf: " + mspfStr;
		SetWindowText(m_mainWnd, windowText.c_str());
		
		// Reset for next average.
		frameCounter = 0;
		timeElapsed += 1.0f;
	}
}

LRESULT IApplication::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return LRESULT();
}

int IApplication::Run()
{
	m_timer.Reset();
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		//Windows Message 
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else //Game Stuff
		{
			m_timer.Tick();
			if (!m_state.m_applicationPaused)
			{
				FrameCalculate();
				Update(m_timer);
				Render(m_timer);
			}
			else
			{
				Sleep(100);
			}
		}
	}
	return (int)msg.wParam;
}

IApplication* IApplication::m_instance = nullptr;
IApplication* IApplication::Instance()
{
	return m_instance;
}
