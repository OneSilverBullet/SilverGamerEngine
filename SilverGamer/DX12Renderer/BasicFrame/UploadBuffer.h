#ifndef SG_UPLOAD_BUFFER
#define SG_UPLOAD_BUFFER

#include "../Common/d3dUtil.h"
#include "DescriptorManager.h"
#include "Device.h"
#include <memory>

template<typename T>
class UploadBuffer
{
public:
	UploadBuffer(ID3D12Device* device, UINT elementNum, bool isConstantBuffer);
	~UploadBuffer();
	UploadBuffer(const UploadBuffer& buffer) = delete;
	UploadBuffer& operator=(const UploadBuffer& buffer) = delete;

	//Function
	ID3D12Resource* GetResource() const;
	void CopyData(int elementIndex, const T& data);

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> m_uploadBuffer;
	BYTE* m_mappedData = nullptr;
	UINT m_elementSize = 0;
	bool mIsConstantBuffer = false;
};


template<typename T>
class ConstantBuffer
{
public:
	ConstantBuffer(Device* device, DescirptorHeap* dh);
	UINT GetConstantBufferSize();
	void CopyData(int elementIndex, const T& data);

private:
	DescirptorHeap* m_bindCBVHeap;
	Device* m_bindDevice;
	UploadBuffer<T>* m_constantBuffer;
};



/*
* UploadBuffer
*/
template<typename T>
inline UploadBuffer<T>::UploadBuffer(ID3D12Device* device, UINT elementNum, bool isConstantBuffer)
{
	m_elementSize = sizeof(T);
	if (isConstantBuffer)
		m_elementSize = d3dUtil::CalcConstantBufferByteSize(m_elementSize);

	//Create Updata Load Buffer
	ThrowIfFailed(device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(m_elementSize * elementNum),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&m_uploadBuffer)));

	//Upload Buffer 
	ThrowIfFailed(m_uploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_mappedData)));
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


/*
* ConstantBuffer
*/
template<typename T>
inline ConstantBuffer<T>::ConstantBuffer(Device* device, DescirptorHeap* dh)
	: m_bindDevice(device), m_bindCBVHeap(dh)
{
	m_constantBuffer = new UploadBuffer<T>(device->GetDevice(), 1, true);
	UINT objectCBByteSize = GetConstantBufferSize();

	D3D12_GPU_VIRTUAL_ADDRESS cbAddress = m_constantBuffer->GetResource()->GetGPUVirtualAddress();

	int boxCBBuffIndex = 0;
	cbAddress += boxCBBuffIndex * objectCBByteSize;

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
	cbvDesc.BufferLocation = cbAddress;
	cbvDesc.SizeInBytes = objectCBByteSize;

	device->GetDevice()->CreateConstantBufferView(
		&cbvDesc,
		m_bindCBVHeap->GetCPUStartDescriptorHandle()
	);
}

template<typename T>
inline UINT ConstantBuffer<T>::GetConstantBufferSize()
{
	return d3dUtil::CalcConstantBufferByteSize(sizeof(T));
}

template<typename T>
void ConstantBuffer<T>::CopyData(int elementIndex, const T& data)
{
	m_constantBuffer->CopyData(elementIndex, data);
}


#endif

