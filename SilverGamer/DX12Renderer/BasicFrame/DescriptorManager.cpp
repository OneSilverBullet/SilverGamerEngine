#include "DescriptorManager.h"


DescirptorHeap::DescirptorHeap(Device* dev,
	D3D12_DESCRIPTOR_HEAP_TYPE heapType /*= D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV*/,
	D3D12_DESCRIPTOR_HEAP_FLAGS heapFlags /*= D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE*/)
	: m_bindDevice(dev), 
	m_descriptorHeapType(heapType),
	m_descriptorHeapFlags(heapFlags)
{
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
	heapDesc.Type = m_descriptorHeapType;
	heapDesc.Flags = m_descriptorHeapFlags;
	heapDesc.NodeMask = 0;
	ThrowIfFailed(m_bindDevice->GetDevice()->
		CreateDescriptorHeap(&heapDesc,
			IID_PPV_ARGS(&m_descriptorHeap)));
}

D3D12_GPU_DESCRIPTOR_HANDLE DescirptorHeap::GetGPUStartDescriptorHandle()
{
	return m_descriptorHeap->GetGPUDescriptorHandleForHeapStart();
}

D3D12_CPU_DESCRIPTOR_HANDLE DescirptorHeap::GetCPUStartDescriptorHandle()
{
	return m_descriptorHeap->GetCPUDescriptorHandleForHeapStart();
}

ID3D12DescriptorHeap* DescirptorHeap::GetHeapPtr()
{
	return m_descriptorHeap.Get();
}
