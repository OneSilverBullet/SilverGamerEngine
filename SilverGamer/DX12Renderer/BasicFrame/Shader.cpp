#include "Shader.h"



/*
* Shader
*/

Shader::Shader()
{
	m_vsByteCode = d3dUtil::CompileShader(L"../ShaderResouce/color.hlsl", nullptr, "VS", "vs_5_0");
	m_psByteCode = d3dUtil::CompileShader(L"../ShaderResouce/color.hlsl", nullptr, "PS", "ps_5_0");
	m_inputLayout =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};
}



/*
* RootSignature
*/
RootSignature::RootSignature(Device* bindDevice) :
	m_bindDevice(bindDevice)
{
	//Create Root Parameter
	CD3DX12_ROOT_PARAMETER slotRootParameters[1]; //one

	//Create Descriptor Table of cbvs
	CD3DX12_DESCRIPTOR_RANGE cbvTable;
	cbvTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);

	//Bind Root Parameter
	slotRootParameters[0].InitAsDescriptorTable(1, &cbvTable);

	//Root Signature Desc of array parameters
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc;
	rootSigDesc.NumParameters = 1;
	rootSigDesc.pParameters = slotRootParameters;
	rootSigDesc.NumStaticSamplers = 0;
	rootSigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//
	Microsoft::WRL::ComPtr<ID3DBlob> serializedRootSig = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorInfo = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(
		&rootSigDesc,
		D3D_ROOT_SIGNATURE_VERSION_1,
		serializedRootSig.GetAddressOf(),
		errorInfo.GetAddressOf()
	);

	if (errorInfo != nullptr)
	{
		::OutputDebugStringA((char*)errorInfo->GetBufferPointer());
	}

	if (SUCCEEDED(hr))
	{
		ThrowIfFailed(
			m_bindDevice->GetDevice()->CreateRootSignature(
				0,
				serializedRootSig->GetBufferPointer(),
				serializedRootSig->GetBufferSize(),
				IID_PPV_ARGS(&m_rootSignature)
			)
		);
	}

}

ID3D12RootSignature* RootSignature::GetRootSig()
{
	return m_rootSignature.Get();
}
