#include "SGSystem.h"
#include <fstream>

/*
* SGSystem
*/
void SilverEngineLib::SGSystem::SystemOutput()
{
	std::cout << "DLL Output" << std::endl;
}

bool SilverEngineLib::SGSystem::IsFileExist(std::string fileDir)
{
    std::fstream fileStream;
    fileStream.open(fileDir.c_str(), std::ios::in);
    if (!fileStream)
    {
        return false;
    }
    return true;
}

/*
* SGGeneralTimer
*/
SilverEngineLib::SGGeneralTimer::SGGeneralTimer()
    : mSecondsPerCount(0.0), mDeltaTime(-1.0), mBaseTime(0),
    mPausedTime(0), mPrevTime(0), mCurrTime(0), mStopped(false)
{        
    __int64 countsPerSec;
    QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
    mSecondsPerCount = 1.0 / (double)countsPerSec;
}

float SilverEngineLib::SGGeneralTimer::TotalTime() const
{
    if (mStopped)
    {
        return (float)(((mStopTime - mPausedTime) - mBaseTime) * mSecondsPerCount);
    }
    else
    {
        return (float)(((mCurrTime - mPausedTime) - mBaseTime) * mSecondsPerCount);
    }
}

float SilverEngineLib::SGGeneralTimer::DeltaTime() const
{
    return (float)mDeltaTime;
}

void SilverEngineLib::SGGeneralTimer::Reset()
{
    __int64 currTime;
    QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

    mBaseTime = currTime;
    mPrevTime = currTime;
    mStopTime = 0;
    mStopped = false;
}

void SilverEngineLib::SGGeneralTimer::Start()
{
    __int64 startTime;
    QueryPerformanceCounter((LARGE_INTEGER*)&startTime);


    // Accumulate the time elapsed between stop and start pairs.
    //
    //                     |<-------d------->|
    // ----*---------------*-----------------*------------> time
    //  mBaseTime       mStopTime        startTime     

    if (mStopped)
    {
        mPausedTime += (startTime - mStopTime);

        mPrevTime = startTime;
        mStopTime = 0;
        mStopped = false;
    }
}

void SilverEngineLib::SGGeneralTimer::Stop()
{
    if (!mStopped)
    {
        __int64 currTime;
        QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

        mStopTime = currTime;
        mStopped = true;
    }
}

void SilverEngineLib::SGGeneralTimer::Tick()
{
    if (mStopped)
    {
        mDeltaTime = 0.0;
        return;
    }

    __int64 currTime;
    QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
    mCurrTime = currTime;

    // Time difference between this frame and the previous.
    mDeltaTime = (mCurrTime - mPrevTime) * mSecondsPerCount;

    // Prepare for next frame.
    mPrevTime = mCurrTime;

    // Force nonnegative.  The DXSDK's CDXUTTimer mentions that if the 
    // processor goes into a power save mode or we get shuffled to another
    // processor, then mDeltaTime can be negative.
    if (mDeltaTime < 0.0)
    {
        mDeltaTime = 0.0;
    }
}
