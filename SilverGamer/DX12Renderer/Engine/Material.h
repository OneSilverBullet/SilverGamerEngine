#ifndef SG_DX12_MATERIAL
#define SG_DX12_MATERIAL

#include "../Common/d3dApp.h"
#include "../Common/MathHelper.h"
#include "../BasicFrame/UploadBuffer.h"

using namespace DirectX;
using namespace DirectX::PackedVector;


namespace SGDX12
{
	struct Texture
	{
		std::string m_name;
		std::wstring m_filename;
		Microsoft::WRL::ComPtr<ID3D12Resource> m_resource;
		Microsoft::WRL::ComPtr<ID3D12Resource> m_uploadHeap;
	};


	struct MaterialConstants
	{
		XMFLOAT4 diffuseAlbedo = { 1.0f, 1.0f, 1.0f,1.0f };
		XMFLOAT3 fresnelR0 = { 0.01f, 0.01f, 0.01f };
		float roughness = 0.25f;
		XMFLOAT4X4 matTransform = MathHelper::Identity4x4();
	};

	struct Material
	{
		std::string m_name;
		int m_matCBIndex = -1;
		int m_diffuseSRVHeapIndex = -1; //index to srv heap for diffuse textures
		int m_normalSRVHeapIndex = -1; //index to srv heap for normal textures
		
		//Does the material need to be updated?
		int m_numFramesDirty = 3;

		//material constant buffer data 
		XMFLOAT4 m_diffuseAlbedo = { 1.0f, 1.0f, 1.0f, 1.0f };
		XMFLOAT3 m_fresnelR0 = {0.01f, 0.01f, 0.01f};
		float roughness = 0.25f;
		XMFLOAT4X4 matTransform = MathHelper::Identity4x4();
	};

}


#endif