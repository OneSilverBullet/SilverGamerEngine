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

ID3D12CommandAllocator* CommandList::GetCommandAllocator()
{
	return m_commandAllocator.Get();
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

void CommandList::Reset(ID3D12PipelineState* pso)
{
	m_commandList->Reset(m_commandAllocator.Get(), pso);
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

void CommandQueue::ExecuteCommandList(CommandList* commandList)
{
	commandList->Close();
	ID3D12CommandList* commandListArrayPtr[] = { commandList->GetCommandList() };
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

void SwapChain::Reset()
{
	for (int i = 0; i < m_buffersCount; ++i)
	{
		m_swapChainResource[i].Reset();
	}
	m_depthStencilBuffer.Reset();
}

void SwapChain::Resize(int clientWidth, int clientHeight, CommandList* bindCommandList)
{
	ThrowIfFailed(m_swapChain->ResizeBuffers(
		m_buffersCount,
		clientWidth, clientHeight,
		m_backBufferFormat,
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));
	m_currentBackBuffer = 0; //reset

	//Create RTVs
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandl(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());
	for (int i = 0; i < m_rtvDescriptorNum; ++i)
	{
		ThrowIfFailed(m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_swapChainResource[i])));
		GetBindDevice()->CreateRenderTargetView(m_swapChainResource[i].Get(), nullptr, rtvHeapHandl);
		rtvHeapHandl.Offset(1, GetBindDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	}

	//Check MSAA Quality
	INT msaaQuality = m_bindCommandQueue->GetBindDevice()->CheckMSAAQualityLevel(m_backBufferFormat);
	
	//Create Depth Stencil Desc
	D3D12_RESOURCE_DESC depthStencilDesc;
	depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthStencilDesc.Alignment = 0;
	depthStencilDesc.Width = clientWidth;
	depthStencilDesc.Height = clientHeight;
	depthStencilDesc.DepthOrArraySize = 1;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	depthStencilDesc.SampleDesc.Count = m_msaaFlag ? 4 : 1;
	depthStencilDesc.SampleDesc.Count = m_msaaFlag ? (msaaQuality - 1) : 0;
	depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE optClear;
	optClear.Format = m_depthStencilFormat;
	optClear.DepthStencil.Depth = 1.0f;
	optClear.DepthStencil.Stencil = 0;
	ThrowIfFailed(GetBindDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthStencilDesc,
		D3D12_RESOURCE_STATE_COMMON,
		&optClear,
		IID_PPV_ARGS(m_depthStencilBuffer.GetAddressOf())));

	//Create Descriptor to mip 0 of depth stencil resource
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Format = m_depthStencilFormat;
	dsvDesc.Texture2D.MipSlice = 0;
	GetBindDevice()->CreateDepthStencilView(m_depthStencilBuffer.Get(), &dsvDesc, DepthStencilView());

	bindCommandList->GetCommandList()->ResourceBarrier(
		1, &CD3DX12_RESOURCE_BARRIER::Transition(
			m_depthStencilBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE));

	m_bindCommandQueue->ExecuteCommandList(bindCommandList);
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

/*
* Fence
*/
Fence::Fence(CommandQueue* commandQueue)
{
	assert(commandQueue != nullptr);
	BindCommandQueue(commandQueue);
	commandQueue->GetBindDevice()->CreateFence(m_fenceInstance);
}

void Fence::BindCommandQueue(CommandQueue* commandQueue)
{
	m_bindQueue = commandQueue;
}

void Fence::FlushCommandQueue()
{
	assert(m_bindQueue != nullptr);

	m_bindQueue->Signal(m_fenceInstance.Get(), m_currentFence);
	if (m_fenceInstance->GetCompletedValue() < m_currentFence)
	{
		HANDLE eventHandle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);
		ThrowIfFailed(m_fenceInstance->SetEventOnCompletion(m_currentFence, eventHandle));
		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}
}
