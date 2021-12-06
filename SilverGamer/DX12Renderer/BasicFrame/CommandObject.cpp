#include "CommandObject.h"
#include <stdlib.h>

/*
* CommandList
*/
CommandList::CommandList(Device* currentDevice,
	D3D12_COMMAND_LIST_TYPE listType /*= D3D12_COMMAND_LIST_TYPE_DIRECT*/)
	: m_bindDevice(currentDevice), m_commandListType(listType)
{
	//Create Command Allocator
	ThrowIfFailed(m_bindDevice->GetDevice()->CreateCommandAllocator(
		m_commandListType,
		IID_PPV_ARGS(m_commandAllocator.GetAddressOf())
	));

	//Create Command List 
	ThrowIfFailed(m_bindDevice->GetDevice()->CreateCommandList(
			0,
			m_commandListType,
			m_commandAllocator.Get(),
			nullptr,
			IID_PPV_ARGS(m_commandList.GetAddressOf())));

	m_commandList->Close();
}

ID3D12GraphicsCommandList* CommandList::GetCommandList()
{
	return m_commandList.Get();
}

D3D12_COMMAND_LIST_TYPE CommandList::GetCommandListType()
{
	return m_commandListType;
}

void CommandList::Close()
{
	m_commandList->Close();
}

void CommandList::Reset()
{
	m_commandList->Reset(m_commandAllocator.Get(), nullptr);
}

/*
* CommandQueue
*/
CommandQueue::CommandQueue(Device* device,
	D3D12_COMMAND_LIST_TYPE listType /*= D3D12_COMMAND_LIST_TYPE_DIRECT*/,
	D3D12_COMMAND_QUEUE_FLAGS queueFlag  /* = D3D12_COMMAND_QUEUE_FLAG_NONE*/)
	: m_bindDevice(device), 
	m_bindCommandListType(listType),
	m_queueFlags(queueFlag)
{
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = m_bindCommandListType;
	queueDesc.Flags = m_queueFlags;
	ThrowIfFailed(m_bindDevice->GetDevice()->CreateCommandQueue(
			&queueDesc,
			IID_PPV_ARGS(m_commandQueue.GetAddressOf())));
}

D3D12_COMMAND_LIST_TYPE CommandQueue::GetBindCommandListType()
{
	return m_bindCommandListType;
}

D3D12_COMMAND_QUEUE_FLAGS CommandQueue::GetQueueFlags()
{
	return m_queueFlags;
}

ID3D12CommandQueue* CommandQueue::GetCommandQueue()
{
	return m_commandQueue.Get();
}

Device* CommandQueue::GetBindDevice()
{
	return m_bindDevice;
}

void CommandQueue::ExecuteCommandList(CommandList commandList)
{
	ID3D12CommandList* commandListArrayPtr[] = { commandList.GetCommandList() };
	m_commandQueue->ExecuteCommandLists(_countof(commandListArrayPtr), commandListArrayPtr);
}

//Process Multi Command Lists
void CommandQueue::ExecuteCommandLists(std::vector<CommandList> commandListArray)
{
	INT arrayNum = static_cast<INT>(commandListArray.size());
	std::vector<ID3D12CommandList*> originDataVec;
	for (int i = 0; i < arrayNum; ++i) {
		originDataVec.push_back(commandListArray[i].GetCommandList());
	}
	ID3D12CommandList** commandListArrayPtr = originDataVec.data();
	m_commandQueue->ExecuteCommandLists(arrayNum, commandListArrayPtr);
}

void CommandQueue::Signal(ID3D12Fence* fence, UINT64 currentFence)
{
	m_commandQueue->Signal(fence, currentFence);
}

/*
* SwapChain
*/
SwapChain::SwapChain(CommandQueue* cq, ApplicationConfig config, HWND wnd)
	: m_bindCommandQueue(cq), m_bindWnd(wnd)
{
	CreateSwapChain(cq, config, wnd);
	CreateRTVDSV();
	//Get The RTV and DSV size
	m_dsvDescriptorNum = m_bindCommandQueue->GetBindDevice()->GetDescriptorHandleIncSizeDSV();
	m_rtvDescriptorNum = m_bindCommandQueue->GetBindDevice()->GetDescriptorHandleIncSizeRTV();
}

ID3D12Resource* SwapChain::GetCurrentChainBuffer()
{
	return m_swapChainResource[m_currentBackBuffer].Get();
}

D3D12_CPU_DESCRIPTOR_HANDLE SwapChain::CurrentBackBufferView() const
{
	//return a array of render target view
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(
		m_rtvHeap->GetCPUDescriptorHandleForHeapStart(),
		m_currentBackBuffer, 
		m_rtvDescriptorNum
	);
}

D3D12_CPU_DESCRIPTOR_HANDLE SwapChain::DepthStencilView() const
{
	//return dsv heap
	return m_dsvHeap->GetCPUDescriptorHandleForHeapStart();
}

//Create Swap Chain
void SwapChain::CreateSwapChain(CommandQueue* cq, ApplicationConfig config, HWND bindWnd)
{
	m_swapChain.Reset();
	INT m4xMsaaQuality = cq->GetBindDevice()->CheckMSAAQualityLevel(m_backBufferFormat);
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	swapChainDesc.BufferDesc.Width = config.GetWidth();
	swapChainDesc.BufferDesc.Height = config.GetHeight();
	swapChainDesc.BufferDesc.RefreshRate.Numerator = config.GetNumerator();
	swapChainDesc.BufferDesc.RefreshRate.Denominator = config.GetDenominator();
	swapChainDesc.BufferDesc.Format = m_backBufferFormat;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SampleDesc.Count = m_msaaFlag ? 4 : 1;
	swapChainDesc.SampleDesc.Quality = m_msaaFlag ? (m4xMsaaQuality - 1) : 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = m_buffersCount;
	swapChainDesc.OutputWindow = m_bindWnd;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	GetBindFactory()->CreateSwapChain(
		cq->GetCommandQueue(), &swapChainDesc, m_swapChain.GetAddressOf()
	);
}

void SwapChain::CreateRTVDSV()
{
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeadDesc;
	rtvHeadDesc.NumDescriptors = m_buffersCount;
	rtvHeadDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeadDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvHeadDesc.NodeMask = 0;
	ThrowIfFailed(
		GetBindDevice()->CreateDescriptorHeap(
			&rtvHeadDesc, IID_PPV_ARGS(m_rtvHeap.GetAddressOf())
		)
	);

	D3D12_DESCRIPTOR_HEAP_DESC dsvHeadDesc;
	dsvHeadDesc.NumDescriptors = 1;
	dsvHeadDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeadDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	dsvHeadDesc.NodeMask = 0;
	ThrowIfFailed(
		GetBindDevice()->CreateDescriptorHeap(
			&dsvHeadDesc, IID_PPV_ARGS(m_dsvHeap.GetAddressOf())
		)
	);
}

ID3D12Device* SwapChain::GetBindDevice()
{
	return m_bindCommandQueue->GetBindDevice()->GetDevice();
}

IDXGIFactory4* SwapChain::GetBindFactory()
{
	return m_bindCommandQueue->GetBindDevice()->GetFactory();
}

