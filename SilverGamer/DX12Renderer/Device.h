#ifndef SG_DEVICE
#define SG_DEVICE

#include "Common/d3dUtil.h"

class Device
{
public:
	Device(D3D_FEATURE_LEVEL featureLevel);
	ID3D12Device* GetDevice() { return m_device.Get(); }
	
private:
	HRESULT CreateDevice();

private:
	bool m_isDebug = true;
	int m_MSAASampleCounter = 4;
	int m_MSAAQualityNum = 0;
	D3D_FEATURE_LEVEL m_featureLevel = D3D_FEATURE_LEVEL_11_0;
	Microsoft::WRL::ComPtr<ID3D12Device> m_device;
	Microsoft::WRL::ComPtr<IDXGIFactory4> m_dxgi; 
};


#endif