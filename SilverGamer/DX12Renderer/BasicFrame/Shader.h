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





class PSO
{
public:



private:


	bool m_isDebug = true;
	int m_MSAASampleCounter = 4;
	int m_MSAAQualityNum = 0;
	D3D_FEATURE_LEVEL m_featureLevel = D3D_FEATURE_LEVEL_11_0;
	Microsoft::WRL::ComPtr<ID3D12Device> m_device;
	Microsoft::WRL::ComPtr<IDXGIFactory4> m_dxgi;
};


#endif