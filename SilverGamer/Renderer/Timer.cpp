#include "..\SGSystemLib\SGSystem.h"
#include "Timer.h"

using namespace std;
using namespace chrono;

void Renderer::SGTimer::Start()
{
	m_prevTime = high_resolution_clock::now();
}

void Renderer::SGTimer::Stop()
{
	m_currTime = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(m_currTime - m_prevTime).count();
	m_deltaTime = double(duration) * 0.000001; //渲染一帧花的时间
}

void SilverEngineLib::SGTimer::Tick()
{
}

float Renderer::SGTimer::GetFrameNum()
{
	return 1.0f / m_deltaTime; //计算得到渲染需要多少帧
}
