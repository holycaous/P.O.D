#include "stdafx.h"
#include <windows.h>
#include "cGameTimer.h"

// Ÿ�̸� Ŭ���� ������ (�ʱ�ȭ)
cGameTimer::cGameTimer()
: mSecondsPerCount(0.0), mDeltaTime(-1.0), mBaseTime(0),
mPausedTime(0), mPrevTime(0), mCurrTime(0), mStopped(false)
{
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	mSecondsPerCount = 1.0 / (double)countsPerSec;
}

// �� �����Ӹ��� ������ �ð��� ���Ѵ�.
void cGameTimer::Tick()
{
	if (mStopped)
	{
		mDeltaTime = 0.0;
		return;
	}
	
	// ����ð��� �����ð��� ���� ���Ͽ�, ������� ������ �ð����� ��ü�Ѵ�.
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	mCurrTime = currTime;
	mDeltaTime = (mCurrTime - mPrevTime)*mSecondsPerCount;
	mPrevTime = mCurrTime;

	// ���� �ð��� 0.0���� �۴ٸ�, 0.0���� �ٽ� �ǵ�����.
	if (mDeltaTime < 0.0)
	{
		mDeltaTime = 0.0;
	}
}

// ������ �ð��� �����Ѵ�.
float cGameTimer::DeltaTime()const
{
	return (float)mDeltaTime;
}

// FPS üũ ����
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

// FPS üũ ����
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

// ��� �����Ǿ�����, �� �ð��� ������ �ǵ����� �Լ�
void cGameTimer::Reset()
{
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	mBaseTime = currTime;
	mPrevTime = currTime;
	mStopTime = 0;
	mStopped = false;
}

// ���콺 �����Ͱ� ���α׷� ���� ������ �ȳ������� üũ�Ͽ�, �ð� ������ �Լ��� �����Ѵ�.
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

