#include "Device.h"

Device::Device(D3D_FEATURE_LEVEL featureLevel) : m_featureLevel(featureLevel)
{
	if (m_isDebug) {
#if defined(DEBUG) || defined(_DEBUG)
		Microsoft::WRL::ComPtr<ID3D12Debug> debugController;
		D3D12GetDebugInterface(IID_PPV_ARGS(&debugController));
		debugController->EnableDebugLayer();
#endif
	}

	//Create Factory
	ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&m_dxgi)));

	//Create Device
	CreateDevice();
}

HRESULT Device::CreateDevice()
{
	assert(m_dxgi != nullptr);
	HRESULT res = D3D12CreateDevice(
		nullptr, //GPU Default
		m_featureLevel, 
		IID_PPV_ARGS(&m_device)
	);

	//WARP
	if (FAILED(res)) {
		Microsoft::WRL::ComPtr<IDXGIAdapter> warpAdapter;
		ThrowIfFailed(m_dxgi->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)));
		ThrowIfFailed(D3D12CreateDevice(
			warpAdapter.Get(),
			m_featureLevel,
			IID_PPV_ARGS(&m_device)
		));
	}
	return res;
}
