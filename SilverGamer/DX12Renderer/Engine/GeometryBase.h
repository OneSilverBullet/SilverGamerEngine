#ifndef SG_GEOMETRY_BASE
#define SG_GEOMETRY_BASE

#include "../Common/d3dApp.h"
#include "../Common/MathHelper.h"
#include "../BasicFrame/UploadBuffer.h"
#include "../Engine/Material.h"

using namespace DirectX;
using namespace DirectX::PackedVector;

struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

//The Constants For per render pass
struct RenderPassConstants
{
	XMFLOAT4X4 m_view = MathHelper::Identity4x4();
	XMFLOAT4X4 m_invView = MathHelper::Identity4x4();
	XMFLOAT4X4 m_proj = MathHelper::Identity4x4();
	XMFLOAT4X4 m_invProj = MathHelper::Identity4x4();
	XMFLOAT4X4 m_viewProj = MathHelper::Identity4x4();
	XMFLOAT4X4 m_invViewProj = MathHelper::Identity4x4();
	XMFLOAT3 m_eyePosW = { 0.0f, 0.0f, 0.0f };
	float m_cvPerObjectPad1 = 0.0f; 
	XMFLOAT2 m_renderTargetSize = { 0.0f, 0.0f };
	XMFLOAT2 m_invRenderTargetSize = { 0.0f, 0.0f };
	float m_nearZ = 0.0f;
	float m_farZ = 0.0f;
	float m_totalTime = 0.0f;
	float m_deltaTime = 0.0f;
};


//The Constant For per object
struct ObjectConstants
{
	XMFLOAT4X4 world = MathHelper::Identity4x4();
};

//SubMesh
struct ISubMesh
{
	UINT m_indexCount = 0;
	UINT m_startIndexLocation = 0; //start vertex
	INT m_baseVertexLocation = 0; //vertexIndex
	
	DirectX::BoundingBox m_boundingBox;
};


//Mesh
struct MeshBase
{
	std::string m_name;
	Microsoft::WRL::ComPtr<ID3DBlob> m_vertexBufferCPU = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> m_indexBufferCPU = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_vertexBufferGPU = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_indexBufferGPU = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_vertexBufferUploader = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_indexBufferUploader = nullptr;

	//Data about Buffers
	UINT m_vertexByteStride = 0; //vertex stride
	UINT m_vertexBufferByteSize = 0; //all byte size
	DXGI_FORMAT m_indexFormat = DXGI_FORMAT_R16_UINT;
	UINT m_indexBufferByteSize = 0; //

	std::unordered_map<std::string, ISubMesh> m_submeshes;

	D3D12_VERTEX_BUFFER_VIEW VertexBufferView() const
	{
		D3D12_VERTEX_BUFFER_VIEW vbv;
		vbv.BufferLocation = m_vertexBufferGPU->GetGPUVirtualAddress();
		vbv.SizeInBytes = m_vertexBufferByteSize;
		vbv.StrideInBytes = m_vertexByteStride;
		return vbv;
	}

	D3D12_INDEX_BUFFER_VIEW IndexBufferView() const
	{
		D3D12_INDEX_BUFFER_VIEW idv;
		idv.BufferLocation = m_indexBufferGPU->GetGPUVirtualAddress();
		idv.SizeInBytes = m_indexBufferByteSize;
		idv.Format = m_indexFormat;
		return idv;
	}

	void DisposeUploaders()
	{
		m_vertexBufferUploader = nullptr;
		m_indexBufferUploader = nullptr;
	}
};


//FrameBuffer
class FrameResource
{
public:
	FrameResource(ID3D12Device* device, UINT passCount, UINT objectCount, UINT materialCount, UINT waveVertCount);
	FrameResource(const FrameResource& rs) = delete;
	FrameResource& operator=(const FrameResource& rs) = delete;
	~FrameResource() {};

	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_commandListAllocator;
	std::unique_ptr<UploadBuffer<RenderPassConstants>> m_passCB = nullptr;
	std::unique_ptr<UploadBuffer<SGDX12::MaterialConstants>> m_materialCB = nullptr;
	std::unique_ptr<UploadBuffer<ObjectConstants>> m_objectCB = nullptr;

	UINT64 m_fence = 0;
};




#endif