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

	//Functions
	void ExecuteCommandList(CommandList commandList);
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


private:


};

/*
* SwapChain: render basic.
*/
class SwapChain
{
public:
	SwapChain();

private:
	void CreateSwapChain();


private:
	static const int m_buffersCount = 2;
	int m_currentBackBuffer = 0;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;

	Microsoft::WRL::ComPtr<ID3D12Resource> m_swapChainResource[m_buffersCount];
	Microsoft::WRL::ComPtr<ID3D12Resource> m_depthStencilBuffer;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_dsvHeap;

	UINT m_rtvDescriptorNum = 0;
	UINT m_dsvDescriptorNum = 0;
};


#endif