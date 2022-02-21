#include "PostEffect.h"



BlurFilter::BlurFilter(ID3D12Device* device, 
	UINT width, UINT height, DXGI_FORMAT format)
{
	m_bindDevice = device;
	m_width = width;
	m_height = height;
	m_format = format;
	BuildResources();
}


std::vector<float> BlurFilter::CalcGuassWeights(float sigma)
{
	float twoSigma2 = 2.0f * sigma * sigma;
	int blurRadius = (int)ceil(2.0f * sigma);
	std::vector<float> weights;
	weights.resize(2 * blurRadius + 1);
	float weightsSum = 0.0f;
	for (int i = -blurRadius; i <= blurRadius; ++i) {
		float x = (float)i;
		weights[i + blurRadius] = expf(-x * x / twoSigma2);
		weightsSum += weights[i + blurRadius];
	}
	for (int i = 0; i < weights.size(); ++i) {
		weights[i] /= weightsSum;
	}
	return weights;
}

//Set blur maps fullfill the descriptor handle.
void BlurFilter::BuildDescriptors()
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = m_format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.Format = m_format;
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
	uavDesc.Texture2D.MipSlice = 0;

	//build 
	m_bindDevice->CreateShaderResourceView(mBlurMap0.Get(), &srvDesc, m_blur0CpuSrv);
	m_bindDevice->CreateUnorderedAccessView(mBlurMap0.Get(), nullptr, &uavDesc, m_blur0CpuUav);


	//build 
	m_bindDevice->CreateShaderResourceView(mBlurMap1.Get(), &srvDesc, m_blur1CpuSrv);
	m_bindDevice->CreateUnorderedAccessView(mBlurMap1.Get(), nullptr, &uavDesc, m_blur1CpuUav);
}

//Create two textures as origin blur resource
void BlurFilter::BuildResources()
{
	D3D12_RESOURCE_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(D3D12_RESOURCE_DESC));
	texDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	texDesc.Alignment = 0;
	texDesc.Width = m_width;
	texDesc.Height = m_height;
	texDesc.DepthOrArraySize = 1;
	texDesc.MipLevels = 1;
	texDesc.Format = m_format;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	texDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

	ThrowIfFailed(m_bindDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&texDesc,
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(&mBlurMap0)));

	ThrowIfFailed(m_bindDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&texDesc,
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(&mBlurMap1)));
}

ID3D12Resource* BlurFilter::Output()
{
	return mBlurMap0.Get();
}

//Division of the descriptors. And fullfile the descriptor of GPU and CPU.
void BlurFilter::BuildDescriptors(CD3DX12_CPU_DESCRIPTOR_HANDLE hCpuDescriptor, 
	CD3DX12_GPU_DESCRIPTOR_HANDLE hGpuDescriptor, 
	UINT descriptorSize)
{
	//load cpu resource from descriptor
	m_blur0CpuSrv = hCpuDescriptor;
	m_blur0CpuUav = hCpuDescriptor.Offset(1, descriptorSize);
	m_blur1CpuSrv = hCpuDescriptor.Offset(1, descriptorSize);
	m_blur1CpuUav = hCpuDescriptor.Offset(1, descriptorSize);

	//load gpu resource from descriptor
	m_blur0GpuSrv = hGpuDescriptor;
	m_blur0GpuUav = hGpuDescriptor.Offset(1, descriptorSize);
	m_blur1GpuSrv = hGpuDescriptor.Offset(1, descriptorSize);
	m_blur1GpuUav = hGpuDescriptor.Offset(1, descriptorSize);

	BuildDescriptors();
}

//
void BlurFilter::OnResize(UINT newWidth, UINT newHeight)
{
	if (m_width != newWidth || m_height != newHeight) {
		m_width = newWidth;
		m_height = newHeight;
		BuildResources();
		BuildDescriptors();
	}
}

void BlurFilter::Execute(ID3D12GraphicsCommandList* cmdList, 
	ID3D12RootSignature* rootSig, 
	ID3D12PipelineState* horzBlurPSO, 
	ID3D12PipelineState* verBlurPSO, 
	ID3D12Resource* input, int blurCount)
{
	std::vector<float> weights = CalcGuassWeights(2.5f);
	int blurRadius = (int)weights.size() / 2;

	//Set new root signature
	cmdList->SetComputeRootSignature(rootSig);
	
	//Upload New RootSignature Parameter
	cmdList->SetComputeRoot32BitConstants(0, 1, &blurRadius, 0); //blur radius
	cmdList->SetComputeRoot32BitConstants(0, (UINT)weights.size(), weights.data(), 1); //guass weights

	//Copy the render target input to mBlurMap0
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(input,
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_COPY_SOURCE));
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mBlurMap0.Get(),
		D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));
	cmdList->CopyResource(mBlurMap0.Get(), input);
	//Write from map1 to map0 using compute shader
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mBlurMap0.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ)); //Set map0 is read state
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mBlurMap1.Get(),
		D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_UNORDERED_ACCESS)); //Set map1 is write state(CS)

	//Execute Compute Shader vicissitudinarily
	for (int i = 0; i < blurCount; ++i)
	{
		//Horizontal Blur Pass
		cmdList->SetPipelineState(horzBlurPSO);
		cmdList->SetComputeRootDescriptorTable(1, m_blur0GpuSrv);
		cmdList->SetComputeRootDescriptorTable(2, m_blur1GpuUav);
		UINT numGroupX = (UINT)ceilf(m_width / 256.0f);
		cmdList->Dispatch(numGroupX, m_height, 1); 

		cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mBlurMap0.Get(),
			D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_UNORDERED_ACCESS));
		cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mBlurMap1.Get(),
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_GENERIC_READ));

		//Vertical Blur Pass
		cmdList->SetPipelineState(verBlurPSO);
		cmdList->SetComputeRootDescriptorTable(1, m_blur1GpuSrv);
		cmdList->SetComputeRootDescriptorTable(2, m_blur0GpuUav);
		UINT numGroupY = (UINT)ceilf(m_height / 256.0f);
		cmdList->Dispatch(m_width, numGroupY, 1);
		cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mBlurMap0.Get(),
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_GENERIC_READ));
		cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mBlurMap1.Get(),
			D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_UNORDERED_ACCESS));
	}
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mBlurMap0.Get(),
		D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_COMMON));
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mBlurMap1.Get(),
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COMMON));
}

SobelFilter::SobelFilter(ID3D12Device* device, UINT width, UINT height, DXGI_FORMAT format)
{
	m_bindDevice = device;
	m_width = width;
	m_height = height;
	m_format = format;
	BuildResources();
}

CD3DX12_GPU_DESCRIPTOR_HANDLE SobelFilter::OutputSrv()
{
	return mhGpuSrv;
}

void SobelFilter::BuildDescriptors(CD3DX12_CPU_DESCRIPTOR_HANDLE hCpuDescriptor, CD3DX12_GPU_DESCRIPTOR_HANDLE hGpuDescriptor, UINT descriptorSize)
{
	//load cpu resource from descriptor
	mhCpuSrv = hCpuDescriptor;
	mhCpuUav = hCpuDescriptor.Offset(1, descriptorSize);

	//load gpu resource from descriptor
	mhGpuSrv = hGpuDescriptor;
	mhGpuUav = hGpuDescriptor.Offset(1, descriptorSize);

	BuildDescriptors();
}

void SobelFilter::OnResize(UINT newWidth, UINT newHeight)
{
	if (m_width != newWidth || m_height != newHeight) {
		m_width = newWidth;
		m_height = newHeight;
		BuildResources();
		BuildDescriptors();
	}
}

void SobelFilter::Execute(ID3D12GraphicsCommandList* cmdList, ID3D12RootSignature* rootSig, 
	ID3D12PipelineState* sobelPSO, CD3DX12_GPU_DESCRIPTOR_HANDLE input)
{
	//Set new root signature
	cmdList->SetComputeRootSignature(rootSig);
	cmdList->SetPipelineState(sobelPSO);

	cmdList->SetComputeRootDescriptorTable(0, input);
	cmdList->SetComputeRootDescriptorTable(1, mhGpuUav);
	UINT numGroupsX = (UINT)ceilf(m_width / 16.0f);
	UINT numGroupsY = (UINT)ceilf(m_height / 16.0f);

	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mOutput.Get(),
		D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_UNORDERED_ACCESS));

	cmdList->Dispatch(numGroupsX, numGroupsY, 1);

	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mOutput.Get(),
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_GENERIC_READ));
}

void SobelFilter::BuildDescriptors()
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = m_format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.Format = m_format;
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
	uavDesc.Texture2D.MipSlice = 0;

	//build 
	m_bindDevice->CreateShaderResourceView(mOutput.Get(), &srvDesc, mhCpuSrv);
	m_bindDevice->CreateUnorderedAccessView(mOutput.Get(), nullptr, &uavDesc, mhCpuUav);
}

void SobelFilter::BuildResources()
{
	D3D12_RESOURCE_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(D3D12_RESOURCE_DESC));
	texDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	texDesc.Alignment = 0;
	texDesc.Width = m_width;
	texDesc.Height = m_height;
	texDesc.DepthOrArraySize = 1;
	texDesc.MipLevels = 1;
	texDesc.Format = m_format;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	texDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

	ThrowIfFailed(m_bindDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&texDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&mOutput)));

}
