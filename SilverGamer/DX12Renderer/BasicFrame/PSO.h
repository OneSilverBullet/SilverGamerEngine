#ifndef SG_PSO
#define SG_PSO

#include "../Common/d3dUtil.h"



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