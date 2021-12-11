
#ifndef SG_DISCRIPTOR_MANAGER
#define SG_DISCRIPTOR_MANAGER

#include "../Common/d3dApp.h"
#include "Device.h"

//For CBV, SRV, UAV
class DescirptorHeap
{
public:
	DescirptorHeap(Device* dev, 
		D3D12_DESCRIPTOR_HEAP_TYPE heapType = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
		D3D12_DESCRIPTOR_HEAP_FLAGS heapFlags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUStartDescriptorHandle();
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUStartDescriptorHandle();
	ID3D12DescriptorHeap* GetHeapPtr();

private:
	Device* m_bindDevice;
	D3D12_DESCRIPTOR_HEAP_TYPE m_descriptorHeapType;
	D3D12_DESCRIPTOR_HEAP_FLAGS m_descriptorHeapFlags;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_descriptorHeap;
};




#endif
