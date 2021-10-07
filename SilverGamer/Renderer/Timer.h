#ifndef SG_TIMER
#define SG_TIMER

#include "RenderSetting.h"

namespace Renderer
{
	class SGTimer
	{
	public:
		SGTimer() {}
		double GetDeltaTime() { return m_deltaTime; }
		void Start();
		void Stop();
		float GetFrameNum();

	private:
		
		std::chrono::steady_clock::time_point m_prevTime;
		std::chrono::steady_clock::time_point m_currTime;
		double m_deltaTime;
	};


}

#endif