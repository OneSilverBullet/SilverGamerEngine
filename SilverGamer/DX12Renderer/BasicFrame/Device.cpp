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


INT Device::CheckMSAAQualityLevel(DXGI_FORMAT backBufferFormat)
{
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
	msQualityLevels.Format = backBufferFormat;
	msQualityLevels.SampleCount = 4;
	msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	msQualityLevels.NumQualityLevels = 0;
	ThrowIfFailed(m_device->CheckFeatureSupport(
		D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
		&msQualityLevels,
		sizeof(msQualityLevels)
	));
	return msQualityLevels.NumQualityLevels;
}

void Device::LogAdapters()
{
	//Loop Adapters
	UINT i = 0;
	IDXGIAdapter* adapter = nullptr;
	std::vector<IDXGIAdapter*> adapterList;
	while (m_dxgi->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_ADAPTER_DESC desc;
		adapter->GetDesc(&desc);

		std::wstring text = L"***Adapter";
		text += desc.Description;
		text += L"\n";

		OutputDebugString(text.c_str());
		adapterList.push_back(adapter);
		++i;
	}

	//Loop Adapter Outputs
	for (size_t i = 0; i < adapterList.size(); ++i){
		LogAdapterOutputs(adapterList[i]);
		ReleaseCom(adapterList[i]);
	}
}

void Device::LogAdapterOutputs(IDXGIAdapter* adapter)
{
	UINT i = 0;
	IDXGIOutput* output = nullptr;
	while (adapter->EnumOutputs(i, &output) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_OUTPUT_DESC outputDesc;
		output->GetDesc(&outputDesc);

		std::wstring text = L"***Output: ";
		text += outputDesc.DeviceName;
		text += L"\n";
		OutputDebugString(text.c_str());

		ReleaseCom(output);
		++i;
	}
}
