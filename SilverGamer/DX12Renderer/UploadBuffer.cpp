#include "UploadBuffer.h"

template<typename T>
inline UploadBuffer<T>::UploadBuffer(ID3D12Device* device, UINT elementNum, bool isConstantBuffer)
{
	m_elementSize = sizeof(T);
	if (isConstantBuffer)
		m_elementSize = d3dUtil::CalcConstantBufferByteSize(m_elementSize);

	//Create Updata Load Buffer
	ThrowIfFailed(device->CreateCommittedResource(
		&CD3D12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3D12_RESOURCE_DESC::Buffer(m_elementSize * elementNum),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,	IID_PPV_ARGS(&m_uploadBuffer)));

	//Upload Buffer 
	ThrowIfFailed(m_uploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(m_mappedData)));
}

template<typename T>
UploadBuffer<T>::~UploadBuffer()
{
	if (m_uploadBuffer != nullptr)
		m_uploadBuffer->Unmap(0, nullptr);
	m_mappedData = nullptr;
}

template<typename T>
ID3D12Resource* UploadBuffer<T>::GetResource() const
{
	return m_uploadBuffer.Get();
}

template<typename T>
void UploadBuffer<T>::CopyData(int elementIndex, const T& data)
{
	memcpy(&m_mappedData[elementIndex * m_elementSize], &data, sizeof(T));
}
