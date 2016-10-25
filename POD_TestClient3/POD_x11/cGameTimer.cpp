#include "stdafx.h"
#include <windows.h>
#include "cGameTimer.h"

// 타이머 클래스 생성자 (초기화)
cGameTimer::cGameTimer()
: mSecondsPerCount(0.0), mDeltaTime(-1.0), mBaseTime(0),
mPausedTime(0), mPrevTime(0), mCurrTime(0), mStopped(false)
{
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	mSecondsPerCount = 1.0 / (double)countsPerSec;
}

// 매 프레임마다 보간된 시간을 구한다.
void cGameTimer::Tick()
{
	if (mStopped)
	{
		mDeltaTime = 0.0;
		return;
	}
	
	// 현재시간과 이전시간의 차를 구하여, 결과값을 보간된 시간으로 대체한다.
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	mCurrTime = currTime;
	mDeltaTime = (mCurrTime - mPrevTime)*mSecondsPerCount;
	mPrevTime = mCurrTime;

	// 계산된 시간이 0.0보다 작다면, 0.0으로 다시 되돌린다.
	if (mDeltaTime < 0.0)
	{
		mDeltaTime = 0.0;
	}
}

// 보간된 시간을 리턴한다.
float cGameTimer::DeltaTime()const
{
	return (float)mDeltaTime;
}

// FPS 체크 시작
void cGameTimer::Start()
{
	__int64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	if (mStopped)
	{
		mPausedTime += (startTime - mStopTime);

		mPrevTime = startTime;
		mStopTime = 0;
		mStopped = false;
	}
}

// FPS 체크 중지
void cGameTimer::Stop()
{
	if (!mStopped)
	{
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		mStopTime = currTime;
		mStopped = true;
	}
}

// 잠시 중지되었을때, 그 시간을 보간해 되돌리는 함수
void cGameTimer::Reset()
{
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	mBaseTime = currTime;
	mPrevTime = currTime;
	mStopTime = 0;
	mStopped = false;
}

// 마우스 포인터가 프로그램 밖을 나갔나 안나갔나를 체크하여, 시간 보간용 함수를 보조한다.
float cGameTimer::TotalTime()const
{
	if (mStopped)
	{
		return (float)(((mStopTime - mPausedTime) - mBaseTime)*mSecondsPerCount);
	}
	else
	{
		return (float)(((mCurrTime - mPausedTime) - mBaseTime)*mSecondsPerCount);
	}
}

