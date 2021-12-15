#ifndef SG_SHADER
#define SG_SHADER

#include "../Common/d3dUtil.h"
#include "Device.h"
#include <vector>


//Shader Encapsulation
class Shader
{
public:
	Shader();
	void* GetVertexShaderBuffer();
	UINT GetVertexShaderBufferSize();
	void* GetPixelShaderBuffer();
	UINT GetPixelShaderBufferSize();
	D3D12_INPUT_ELEMENT_DESC* GetInputLayoutData();
	UINT GetInputLayoutNum();


private:
	Microsoft::WRL::ComPtr<ID3DBlob> m_vsByteCode;
	Microsoft::WRL::ComPtr<ID3DBlob> m_psByteCode;
	std::vector<D3D12_INPUT_ELEMENT_DESC> m_inputLayout;

};





//The layout structure of shader accessible resources.
class RootSignature
{
public:
	RootSignature(Device* bindDevice);
	ID3D12RootSignature* GetRootSig();

private:
	Device* m_bindDevice;
	Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature = nullptr;
};






#endif