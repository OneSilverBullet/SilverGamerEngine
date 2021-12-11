#ifndef SG_GEOMETRY_BASE
#define SG_GEOMETRY_BASE

#include "../Common/d3dApp.h"
#include "../Common/MathHelper.h"

using namespace DirectX;
using namespace DirectX::PackedVector;

struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

//
struct ObjectConstants
{
	XMFLOAT4X4 worldViewProj = MathHelper::Identity4x4();
};

//SubMesh
struct SubMesh
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
	DXGI_FORMAT indexFormat = DXGI_FORMAT_R16_UINT;
	UINT m_indexBufferByteSize = 0; //
	UINT m_indexBufferByteSize = 0; //

	std::unordered_map<std::string, SubMesh> m_submeshes;

	D3D12_VERTEX_BUFFER_VIEW VertexBufferView() const
	{
		D3D12_VERTEX_BUFFER_VIEW vbv;
		vbv.BufferLocation = m_vertexBufferGPU->GetGPUVirtualAddress();
		vbv.SizeInBytes = m_vertexByteStride;
		vbv.StrideInBytes = m_vertexBufferByteSize;
		return vbv;
	}

	D3D12_INDEX_BUFFER_VIEW IndexBufferView() const
	{
		D3D12_INDEX_BUFFER_VIEW idv;
		idv.BufferLocation = m_indexBufferGPU->GetGPUVirtualAddress();
		idv.SizeInBytes = m_indexBufferByteSize;
		idv.Format = indexFormat;
		return idv;
	}

	void DisposeUploaders()
	{
		m_vertexBufferUploader = nullptr;
		m_indexBufferUploader = nullptr;
	}
};

#endif