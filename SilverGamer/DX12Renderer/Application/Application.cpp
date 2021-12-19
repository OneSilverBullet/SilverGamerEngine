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
	m_mainWnd = CreateWindow(L"SilverGamer", m_appConfig.GetMainWndCaption().c_str(),
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
#if defined(DEBUG) || defined(_DEBUG) 
	// Enable the D3D12 debug layer.
	{
		Microsoft::WRL::ComPtr<ID3D12Debug> debugController;
		ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
		debugController->EnableDebugLayer();
	}
#endif

	m_device = new Device(D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0);
	m_commandQueue = new CommandQueue(m_device);
	m_commandList = new CommandList(m_device);
	m_fence = new Fence(m_commandQueue); //Bind a fence to the commandQueue.


	// Release the previous swapchain we will be recreating.
	/*
	Microsoft::WRL::ComPtr<IDXGISwapChain> tSwapChain;
	tSwapChain.Reset();
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = 800;
	sd.BufferDesc.Height = 600;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2;
	sd.OutputWindow = m_mainWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// Note: Swap chain uses queue to perform flush.
	ThrowIfFailed(m_device->GetFactory()->CreateSwapChain(
		m_commandQueue->GetCommandQueue(),
		&sd,
		tSwapChain.GetAddressOf()));

	*/
	m_swapChain = new SwapChain(m_commandQueue, m_device, m_appConfig, m_mainWnd);
	return true;
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

void IApplication::OnResize()
{
	assert(m_device != nullptr);
	assert(m_swapChain != nullptr);
	assert(m_commandList != nullptr);
	//Clear the command of current command Queue
	m_fence->FlushCommandQueue();
	m_commandList->Reset();
	m_swapChain->Resize(m_state.m_clientWidth, m_state.m_clientHeigh, m_commandList);
	//Flush the resize command
	m_fence->FlushCommandQueue(); 

	//Create the viewport transform
	m_screenViewport.TopLeftX = 0;
	m_screenViewport.TopLeftY = 0;
	m_screenViewport.Width = static_cast<float>(m_state.m_clientWidth);
	m_screenViewport.Height = static_cast<float>(m_state.m_clientHeigh);
	m_screenViewport.MinDepth = 0.0f;
	m_screenViewport.MaxDepth = 1.0f;

	m_scissorRect = { 0, 0, m_state.m_clientWidth, m_state.m_clientHeigh };
}

LRESULT IApplication::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	//Current Window Active Information
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE) {
			m_state.m_applicationPaused = true;
			m_timer.Stop(); 
		}
		else {
			m_state.m_applicationPaused = false;
			m_timer.Start();
		}
		return 0;

	//Window Size Information
	case WM_SIZE:
		m_state.m_clientWidth = LOWORD(lParam);
		m_state.m_clientHeigh = HIWORD(lParam);
		if (m_device != nullptr) {
			if (wParam == SIZE_MINIMIZED){
				m_state.m_applicationPaused = true;
				m_state.m_minimized = true;
				m_state.m_maximized = false;
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				m_state.m_applicationPaused = false;
				m_state.m_minimized = false;
				m_state.m_maximized = true;
				OnResize();
			}
			else if (wParam == SIZE_RESTORED)
			{
				if (m_state.m_minimized) {
					m_state.m_applicationPaused = false;
					m_state.m_minimized = false;
					OnResize();
				}
				else if (m_state.m_maximized) {
					m_state.m_applicationPaused = false;
					m_state.m_maximized = false;
					OnResize();
				}
				else if (m_state.m_resizing) {

				}
				else
					OnResize();
			}
		}
		return 0;

	//grab the resize bar
	case WM_ENTERSIZEMOVE:
		m_state.m_applicationPaused = true;
		m_state.m_resizing = true;
		m_timer.Stop();
		return 0;

	//release the resize bars
	case WM_EXITSIZEMOVE:
		m_state.m_applicationPaused = false;
		m_state.m_resizing = false;
		m_timer.Start();
		OnResize();
		return 0;

	//the window is destroyed
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_MENUCHAR:
		// Don't beep when we alt-enter.
		return MAKELRESULT(0, MNC_CLOSE);

		// Catch this message so to prevent the window from becoming too small.
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
		return 0;

	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_MOUSEMOVE:
		OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_KEYUP:
		if (wParam == VK_ESCAPE)
		{
			PostQuitMessage(0);
		}

		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

IApplication::IApplication(HINSTANCE hInstance):
	m_appInstance(hInstance)
{
	assert(m_instance == nullptr);
	m_instance = this;
}

IApplication::~IApplication()
{
	if (m_device != nullptr)
		m_fence->FlushCommandQueue();
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

bool IApplication::Initialize()
{
	if (!InitMainWindow())
		return false;

	if (!InitGraphics())
		return false;

	// Do the initial resize code.
	OnResize();
	return true;
}
