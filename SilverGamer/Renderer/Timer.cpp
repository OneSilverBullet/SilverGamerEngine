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
	m_deltaTime = double(duration) * 0.000001;
}
