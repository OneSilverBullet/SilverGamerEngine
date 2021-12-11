#ifndef SG_COMMAND_OBJECT
#define SG_COMMAND_OBJECT

#include "Device.h"
#include <vector>

//CommandList Capusule
class CommandList
{
public:
	CommandList(Device* currentDevice, D3D12_COMMAND_LIST_TYPE value = D3D12_COMMAND_LIST_TYPE_DIRECT);
	ID3D12GraphicsCommandList* GetCommandList();
	D3D12_COMMAND_LIST_TYPE GetCommandListType();
	//Function
	void Close();
	void Reset();

private:
	Device* m_bindDevice;
	D3D12_COMMAND_LIST_TYPE m_commandListType;
	//For Convienance, the commandList and commandAllocator work as a pair.
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_commandAllocator;
};


//Command Queue
class CommandQueue
{
public:
	CommandQueue(Device* device, 
		D3D12_COMMAND_LIST_TYPE listType = D3D12_COMMAND_LIST_TYPE_DIRECT,
		D3D12_COMMAND_QUEUE_FLAGS queueFlag = D3D12_COMMAND_QUEUE_FLAG_NONE);
	D3D12_COMMAND_LIST_TYPE GetBindCommandListType();
	D3D12_COMMAND_QUEUE_FLAGS GetQueueFlags();
	ID3D12CommandQueue* GetCommandQueue();

	Device* GetBindDevice();

	//Functions
	void ExecuteCommandList(CommandList* commandList);
	void ExecuteCommandLists(std::vector<CommandList> commandListArray);
	void Signal(ID3D12Fence* fence, UINT64 currentFence);

private:
	D3D12_COMMAND_LIST_TYPE m_bindCommandListType;
	D3D12_COMMAND_QUEUE_FLAGS m_queueFlags;
	Device* m_bindDevice;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_commandQueue;
};

/*
* Application Config: Application Information
*/
class ApplicationConfig
{
public:
	void LoadFromConfigFile(); //Load App Information From File
	std::wstring GetMainWndCaption() { return mainWndCaption; }
	int GetWidth() { return m_clientWidth; }
	int GetHeight() { return m_clientHeight; }
	int GetNumerator() { return m_numerator; }
	int GetDenominator() { return m_denominator; }
	float GetAspectRatio(){return  static_cast<float>(m_clientWidth) / m_clientHeight; }
private:
	int m_clientWidth = 1280;
	int m_clientHeight = 720;
	int m_numerator = 60;
	int m_denominator = 1;
	std::wstring mainWndCaption = L"SilverGamer";

};

/*
* SwapChain: render basic.
*/
class SwapChain
{
public:
	SwapChain(CommandQueue* cq, ApplicationConfig config, HWND bindWnd);

	ID3D12Resource* GetCurrentChainBuffer();
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView() const;
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() const;
	void Reset(); 
	void Resize(int clientWidth, int clientHeight, CommandList* bindCmd);

private:
	void CreateSwapChain(CommandQueue* cq, ApplicationConfig config, HWND bindWnd);
	void CreateRTVDSV();
	ID3D12Device* GetBindDevice();
	IDXGIFactory4* GetBindFactory();
private:
	CommandQueue* m_bindCommandQueue;
	HWND m_bindWnd; 
	bool m_msaaFlag = true;

	static const int m_buffersCount = 2;
	int m_currentBackBuffer = 0;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;

	DXGI_FORMAT m_backBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT m_depthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	Microsoft::WRL::ComPtr<ID3D12Resource> m_swapChainResource[m_buffersCount];
	Microsoft::WRL::ComPtr<ID3D12Resource> m_depthStencilBuffer;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_dsvHeap;

	UINT m_rtvDescriptorNum = 0;
	UINT m_dsvDescriptorNum = 0;
};


/*
* Fence
*/
class Fence
{
public:
	Fence(CommandQueue* commandQueue);
	void BindCommandQueue(CommandQueue* commandQueue);
	void FlushCommandQueue();

private:
	CommandQueue* m_bindQueue;
	UINT64 m_currentFence = 0;
	Microsoft::WRL::ComPtr<ID3D12Fence> m_fenceInstance;
};

#endif