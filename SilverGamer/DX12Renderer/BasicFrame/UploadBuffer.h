#ifndef SG_UPLOAD_BUFFER
#define SG_UPLOAD_BUFFER

#include "Common/d3dUtil.h"

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




#endif

