#include "GeometryBase.h"

FrameResource::FrameResource(ID3D12Device* device, UINT passCount, UINT objectCount, UINT waveVertCount)
{
	ThrowIfFailed(
		device->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(m_commandListAllocator.GetAddressOf()))
	);

	m_passCB = std::make_unique<UploadBuffer<RenderPassConstants>>(device, passCount, true);
	m_objectCB = std::make_unique<UploadBuffer<ObjectConstants>>(device, objectCount, true);
}
