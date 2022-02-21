
#pragma once

#include "../Common/d3dUtil.h"


class BlurFilter
{
public:
	BlurFilter(ID3D12Device* device, 
		UINT width, UINT height, DXGI_FORMAT format);
	BlurFilter(const BlurFilter&) = delete;
	BlurFilter& operator=(const BlurFilter&) = delete;
	~BlurFilter() = default;

	//
	ID3D12Resource* Output();
	void BuildDescriptors(
		CD3DX12_CPU_DESCRIPTOR_HANDLE hCpuDescriptor,
		CD3DX12_GPU_DESCRIPTOR_HANDLE hGpuDescriptor,
		UINT descriptorSize
	);

	void OnResize(UINT newWidth, UINT newHeight);

	void Execute(
		ID3D12GraphicsCommandList* cmdList,
		ID3D12RootSignature* rootSig,
		ID3D12PipelineState* horzBlurPSO,
		ID3D12PipelineState* verBlurPSO,
		ID3D12Resource* input,
		int blurCount
	);

private:
	std::vector<float> CalcGuassWeights(float sigma);
	void BuildDescriptors();
	void BuildResources();


private:
	const int m_maxBlurRadius = 5;
	ID3D12Device* m_bindDevice = nullptr;
	UINT m_width;
	UINT m_height;
	DXGI_FORMAT m_format = DXGI_FORMAT_R8G8B8A8_UNORM;

	CD3DX12_CPU_DESCRIPTOR_HANDLE m_blur0CpuSrv;
	CD3DX12_CPU_DESCRIPTOR_HANDLE m_blur0CpuUav;

	CD3DX12_CPU_DESCRIPTOR_HANDLE m_blur1CpuSrv;
	CD3DX12_CPU_DESCRIPTOR_HANDLE m_blur1CpuUav;

	CD3DX12_GPU_DESCRIPTOR_HANDLE m_blur0GpuSrv;
	CD3DX12_GPU_DESCRIPTOR_HANDLE m_blur0GpuUav;
	
	CD3DX12_GPU_DESCRIPTOR_HANDLE m_blur1GpuSrv;
	CD3DX12_GPU_DESCRIPTOR_HANDLE m_blur1GpuUav;

	Microsoft::WRL::ComPtr<ID3D12Resource> mBlurMap0 = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> mBlurMap1 = nullptr;
};


class SobelFilter
{

public:
	SobelFilter(ID3D12Device* device,
		UINT width, UINT height, DXGI_FORMAT format);
	SobelFilter(const SobelFilter&) = delete;
	SobelFilter& operator=(const SobelFilter&) = delete;
	~SobelFilter() = default;

	//
	CD3DX12_GPU_DESCRIPTOR_HANDLE OutputSrv();
	void BuildDescriptors(
		CD3DX12_CPU_DESCRIPTOR_HANDLE hCpuDescriptor,
		CD3DX12_GPU_DESCRIPTOR_HANDLE hGpuDescriptor,
		UINT descriptorSize
	);

	void OnResize(UINT newWidth, UINT newHeight);
	void Execute(ID3D12GraphicsCommandList* cmdList, ID3D12RootSignature* rootSig,
		ID3D12PipelineState* sobelPSO, CD3DX12_GPU_DESCRIPTOR_HANDLE input);

private:
	void BuildDescriptors();
	void BuildResources();

private:
	ID3D12Device* m_bindDevice = nullptr;
	UINT m_width;
	UINT m_height;
	DXGI_FORMAT m_format = DXGI_FORMAT_R8G8B8A8_UNORM;

	CD3DX12_CPU_DESCRIPTOR_HANDLE mhCpuSrv;
	CD3DX12_CPU_DESCRIPTOR_HANDLE mhCpuUav;

	CD3DX12_GPU_DESCRIPTOR_HANDLE mhGpuSrv;
	CD3DX12_GPU_DESCRIPTOR_HANDLE mhGpuUav;


	Microsoft::WRL::ComPtr<ID3D12Resource> mOutput = nullptr;//Map0 Copy From Input};
};