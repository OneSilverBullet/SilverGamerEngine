#pragma once

#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <memory.h>
#include <assert.h>
#include <sys/stat.h>
#include <atlsimpstr.h>
#include  "MacroLib.h"

namespace SilverEngineLib
{

	class SG_API SGSystem {
	public:
		void SystemOutput();
		static bool IsFileExist(std::string fileDir);
	};


	class SG_API SGGeneralTimer
	{
	public:
		SGGeneralTimer();
		float TotalTime()const; // in seconds
		float DeltaTime()const; // in seconds
		void Reset(); // Call before message loop.
		void Start(); // Call when unpaused.
		void Stop();  // Call when paused.
		void Tick();  // Call every frame.

	private:
		double mSecondsPerCount;
		double mDeltaTime;

		__int64 mBaseTime;
		__int64 mPausedTime;
		__int64 mStopTime;
		__int64 mPrevTime;
		__int64 mCurrTime;

		bool mStopped;
	};


}