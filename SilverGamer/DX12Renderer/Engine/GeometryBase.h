#ifndef SG_GEOMETRY_BASE
#define SG_GEOMETRY_BASE

#include "../Common/d3dApp.h"
#include "../Common/MathHelper.h"
#include "../BasicFrame/UploadBuffer.h"
#include "../Engine/Material.h"
#include "../Engine/Light.h"

using namespace DirectX;
using namespace DirectX::PackedVector;

struct VertexData
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT2 TexC;
};


struct Vertex
{
	Vertex() {}
	Vertex(
		const XMFLOAT3& pos,
		const XMFLOAT3& norm,
		const XMFLOAT3& tangent,
		const XMFLOAT2& tex
	) :
		Position(pos),
		Normal(norm),
		TangentU(tangent),
		TexC(tex)
	{}
	Vertex(
		float px, float py, float pz,
		float nx, float ny, float nz,
		float tx, float ty, float tz,
		float u, float v
	):
		Position(px, py, pz),
		Normal(nx,ny,nz),
		TangentU(tx, ty, tz),
		TexC(u, v)
	{}

	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT3 TangentU;
	XMFLOAT2 TexC;
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
	XMFLOAT4 m_ambientLight = { 0.0f, 0.0f, 0.0f, 1.0f };
	DirectX::XMFLOAT4 FogColor = { 0.7f, 0.7f, 0.7f, 1.0f };
	float gFogStart = 1.0f;
	float gFogRange = 10.0f;
	DirectX::XMFLOAT2 cbPerObjectPad2;
	SGDX12::Light m_lights[MAX_LIGHT_NUM];
};


//The Constant For per object
struct ObjectConstants
{
	XMFLOAT4X4 world = MathHelper::Identity4x4();
	XMFLOAT4X4 texTransform = MathHelper::Identity4x4();
};

//SubMesh
struct ISubMesh
{
	UINT m_indexCount = 0;
	UINT m_startIndexLocation = 0; //start vertex
	INT m_baseVertexLocation = 0; //vertexIndex
	
	DirectX::BoundingBox m_boundingBox;
};


//DX12 Mesh Data Internal
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

//Mesh Data
struct MeshData
{
	std::vector<Vertex> m_vertices;
	std::vector<UINT> m_indices;
	std::vector<UINT16>& GetIndices16() {
		if (m_indices16.empty()) {
			m_indices16.resize(m_indices.size());
			for (int i = 0; i < m_indices.size(); ++i) {
				m_indices16[i] = static_cast<UINT16>(m_indices[i]);
			}
		}
		return m_indices16;
	}

private:
	std::vector<UINT16> m_indices16;

};


class GeometryGenerator
{
public:
	static MeshData* Box(float width, float height, float depth, UINT numSubdivision);


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