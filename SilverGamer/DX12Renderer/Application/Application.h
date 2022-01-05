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

	float AspectRatio() { return (float)m_clientWidth / (float)m_clientHeigh; }

};

//Basic Application Class
class IApplication
{
public:
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	IApplication(HINSTANCE hInstance);
	virtual ~IApplication();
	int Run();
	static IApplication* Instance();


protected:

	virtual bool Initialize();
	virtual void Update(const SilverEngineLib::SGGeneralTimer& timer) = 0;
	virtual void Render(const SilverEngineLib::SGGeneralTimer& timer) = 0;
	// Convenience overrides for handling mouse input.
	virtual void OnMouseDown(WPARAM btnState, int x, int y) { }
	virtual void OnMouseUp(WPARAM btnState, int x, int y) { }
	virtual void OnMouseMove(WPARAM btnState, int x, int y) { }
	

protected:
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView();
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView();
	ID3D12Resource* CurrentBackBuffer();

	void FlushCommandQueue();

	bool InitMainWindow();
	bool InitGraphics(); 
	void FrameCalculate();
	virtual void OnResize(); //The Size changed function

	static IApplication* m_instance;

protected:
	HINSTANCE m_appInstance;
	HWND m_mainWnd;

	ApplicationState m_state;
	ApplicationConfig m_appConfig;
	SilverEngineLib::SGGeneralTimer m_timer;

	D3D12_VIEWPORT m_screenViewport;
	D3D12_RECT m_scissorRect;

	Microsoft::WRL::ComPtr<IDXGIFactory4> mdxgiFactory;
	Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
	Microsoft::WRL::ComPtr<ID3D12Device> md3dDevice;

	Microsoft::WRL::ComPtr<ID3D12Fence> mFence;
	UINT64 mCurrentFence = 0;

	Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCommandQueue;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mDirectCmdListAlloc;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList;

	static const int SwapChainBufferCount = 2;
	int mCurrBackBuffer = 0;
	Microsoft::WRL::ComPtr<ID3D12Resource> mSwapChainBuffer[SwapChainBufferCount];
	Microsoft::WRL::ComPtr<ID3D12Resource> mDepthStencilBuffer;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mRtvHeap;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDsvHeap;
	
	D3D_DRIVER_TYPE md3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
	DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	UINT mRtvDescriptorSize = 0;
	UINT mDsvDescriptorSize = 0;
	UINT mCbvSrvUavDescriptorSize = 0;

	bool      m4xMsaaState = false;    // 4X MSAA enabled
	UINT      m4xMsaaQuality = 0;      // quality level of 4X MSAA
	int mClientWidth = 1280;
	int mClientHeight = 720;

	Fence* m_fence;
};

#endif