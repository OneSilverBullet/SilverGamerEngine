#ifndef SG_PSO
#define SG_PSO

#include "../Common/d3dUtil.h"
#include "Shader.h"
#include "CommandObject.h"

class PSO
{
public:
	PSO(Device* bindDevice, SwapChain* swapChain, Shader* shader, bool msaaFlag = true);
	ID3D12PipelineState* GetPSO() { return m_pipelineStateObject.Get(); }

private:
	SwapChain* m_bindSwapChain;
	Device* m_bindDevice;
	Shader* m_bindShader;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineStateObject = nullptr;
};


#endif