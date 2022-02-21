#ifndef SG_GRAPHICS
#define SG_GRAPHICS

#include "ApplicationConfig.h"
#include "../Common/d3dUtil.h"
#include "../Common/MathHelper.h"
#include "../Engine/GeometryBase.h"
#include "../Engine/Material.h"

#define SG_NUM_FRAME_RESOURCES 3

using namespace DirectX;
using namespace DirectX::PackedVector;

struct RenderItem
{
	RenderItem() = default;
	XMFLOAT4X4 m_world = MathHelper::Identity4x4();
	XMFLOAT4X4 m_texTransform = MathHelper::Identity4x4();
	int m_numFrameDirty = 3;

	UINT m_objectCBIndex = -1;//index to GPU constant buffer corresponding to this renderItem.
	MeshBase* geo = nullptr;
	SGDX12::Material* mat = nullptr;

	//Draw Indexed Instanced Parameters
	D3D12_PRIMITIVE_TOPOLOGY m_primitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	UINT m_indexCount = 0;
	UINT m_startIndexLocation = 0;
	int m_baseVertexLocation = 0;
};


enum class RenderLayer : int
{
	Opaque = 0,
	Sprite = 1,
	Count
};


struct SG_D3D_SHADER_MACRO
{
	LPCSTR Name;
	LPCSTR Definition;
};


class RenderTarget
{
public:
	RenderTarget(ID3D12Device* device,
		UINT width, UINT height,
		DXGI_FORMAT format);
	RenderTarget(const RenderTarget&) = delete;
	RenderTarget& operator=(const RenderTarget&) = delete;
	~RenderTarget() = default;

	//functions
	ID3D12Resource* Resource();
	CD3DX12_GPU_DESCRIPTOR_HANDLE Srv();
	CD3DX12_CPU_DESCRIPTOR_HANDLE Rtv();
	void BuildDescriptors(
		CD3DX12_CPU_DESCRIPTOR_HANDLE hCpuSrv,
		CD3DX12_GPU_DESCRIPTOR_HANDLE hGpuSrv,
		CD3DX12_CPU_DESCRIPTOR_HANDLE hCpuRtv
	);
	void OnResize(UINT newWidth, UINT newHeight); //resize the render target

private:
	void BuildDescriptors();
	void BuildResource();

private:
	ID3D12Device* m_d3dDevice = nullptr;

	UINT m_width = 0;
	UINT m_height = 0;
	DXGI_FORMAT m_format = DXGI_FORMAT_R8G8B8A8_UNORM;

	CD3DX12_CPU_DESCRIPTOR_HANDLE mhCpuSrv; //Used for render as shader resource
	CD3DX12_GPU_DESCRIPTOR_HANDLE mhGpuSrv; 
	CD3DX12_CPU_DESCRIPTOR_HANDLE mhCpuRtv; //Used for render as render target

	Microsoft::WRL::ComPtr<ID3D12Resource> m_offscreenTex = nullptr;
};


#endif