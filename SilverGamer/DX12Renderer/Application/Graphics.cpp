#ifndef SG_GRAPHICS_APPLICATION
#define SG_GRAPHICS_APPLICATION

#include "ApplicationConfig.h"
#include "Graphics.h"

//Basic Application Class
class IApplication
{
public:
	bool InitMainWindow(); 


private:
	HINSTANCE m_appInstance;
	HWND m_mainWnd;

	Device* m_device;
	SwapChain* m_swapChain;
	CommandQueue* m_commandQueue;
	CommandList* m_commandList;
	ApplicationConfig m_appConfig;


};




/*
* RenderTarget
*/
RenderTarget::RenderTarget(ID3D12Device* device, UINT width, UINT height, DXGI_FORMAT format)
	: m_d3dDevice(device), m_width(width), m_height(height), m_format(format)
{
	BuildDescriptors(); //Create the descriptors of the resource
}

ID3D12Resource* RenderTarget::Resource()
{
	return m_offscreenTex.Get();
}

CD3DX12_GPU_DESCRIPTOR_HANDLE RenderTarget::Srv()
{
	return mhGpuSrv;
}

CD3DX12_CPU_DESCRIPTOR_HANDLE RenderTarget::Rtv()
{
	return mhCpuRtv;
}

void RenderTarget::BuildDescriptors(
	CD3DX12_CPU_DESCRIPTOR_HANDLE hCpuSrv, 
	CD3DX12_GPU_DESCRIPTOR_HANDLE hGpuSrv, 
	CD3DX12_CPU_DESCRIPTOR_HANDLE hCpuRtv)
{
	mhCpuSrv = hCpuSrv;
	mhGpuSrv = hGpuSrv;
	mhCpuRtv = hCpuRtv;
	BuildDescriptors(); 
}

void RenderTarget::OnResize(UINT newWidth, UINT newHeight)
{
	if (m_width != newWidth || m_height != newHeight)
	{
		m_width = newWidth;
		m_height = newHeight;
		BuildResource();
		BuildDescriptors();
	}
}

void RenderTarget::BuildDescriptors()
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = m_format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	m_d3dDevice->CreateShaderResourceView(m_offscreenTex.Get(), &srvDesc, mhCpuSrv);
	m_d3dDevice->CreateRenderTargetView(m_offscreenTex.Get(), nullptr, mhCpuRtv);
}

void RenderTarget::BuildResource()
{
	//Create a empty texture resource to store render result
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
	texDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	//Create Commited Resource
	ThrowIfFailed(
		m_d3dDevice->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			&texDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_offscreenTex))
	);
}



#endif