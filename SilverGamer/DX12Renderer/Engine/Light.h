#ifndef SG_DX12_LIGHT
#define SG_DX12_LIGHT

#include "../Common/d3dApp.h"
#include "../Common/MathHelper.h"

using namespace DirectX;
using namespace DirectX::PackedVector;

#define MAX_LIGHT_NUM 16

namespace SGDX12
{
	struct Light
	{
		XMFLOAT3 m_strength = { 0.5f, 0.5f, 0.5f };
		float m_falloffStart = 1.0f;				  //point/spot light need
		XMFLOAT3 m_position = { 0.0f, 0.0f, 0.0f }; 
		float m_falloffEnd = 10.0f;
		XMFLOAT3 m_direction = { 0.0f, -1.0f, 0.0f }; //direction/spot light need 
		float m_spotPower = 64.0f; //spot light
	};

}


#endif