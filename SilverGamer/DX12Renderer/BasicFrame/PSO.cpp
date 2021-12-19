#include "PSO.h"

PSO::PSO(Device* bindDevice, SwapChain* swapChain, Shader* shader, RootSignature* sig, bool msaaFlag)
	: m_bindDevice(bindDevice), m_bindSwapChain(swapChain), m_bindShader(shader), m_bindRootSig(sig)
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	ZeroMemory(&psoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	//Load Shader Information
	psoDesc.InputLayout.pInputElementDescs = shader->GetInputLayoutData();
	psoDesc.InputLayout.NumElements = shader->GetInputLayoutNum();
	psoDesc.pRootSignature = m_bindRootSig->GetRootSig();
	psoDesc.VS.pShaderBytecode = reinterpret_cast<BYTE*>(shader->GetVertexShaderBuffer());
	psoDesc.VS.BytecodeLength = shader->GetVertexShaderBufferSize();
	psoDesc.PS.pShaderBytecode = reinterpret_cast<BYTE*>(shader->GetPixelShaderBuffer());
	psoDesc.PS.BytecodeLength = shader->GetPixelShaderBufferSize();

	//
	INT m4xMsaaQuality = m_bindDevice->CheckMSAAQualityLevel(m_bindSwapChain->GetBackBufferFormat());

	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = m_bindSwapChain->GetBackBufferFormat();
	psoDesc.SampleDesc.Count = 1; //msaaFlag ? 4 : 1;
	psoDesc.SampleDesc.Quality = 0; // msaaFlag ? (m4xMsaaQuality - 1) : 0;
	psoDesc.DSVFormat = m_bindSwapChain->GetDepthBufferFormat();

	//Create the device
	ThrowIfFailed(
		m_bindDevice->GetDevice()->CreateGraphicsPipelineState(&psoDesc, 
			IID_PPV_ARGS(&m_pipelineStateObject))
	);
}
