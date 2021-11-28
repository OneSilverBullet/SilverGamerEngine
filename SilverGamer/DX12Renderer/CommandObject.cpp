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
SwapChain::SwapChain()
{


}

void SwapChain::CreateSwapChain()
{
	m_swapChain.Reset();

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	swapChainDesc.BufferDesc.Width = 


}
