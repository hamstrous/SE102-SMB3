#include "Timer.h"

#include "Timer.h"


Timer::Timer(ULONGLONG timeSpan)
{
	this->timeSpan = timeSpan;
	timeStart = -1;
	started = false;
}

Timer::Timer()
{
	timeStart = -1;
	started = false;
}

void Timer::Start()
{
	timeStart = GetTickCount64();
	started = true;
}

bool Timer::IsDone()
{
	if (timeStart == -1) return false;
	ULONGLONG elapsed = GetTickCount64() - timeStart;
	if (elapsed >= timeSpan)
	{
		started = false;
		timeStart = -1;
		return true;
	}
}

void Timer::Reset()
{
	timeStart = -1;
	started = false;
}

ULONGLONG Timer::ElapsedTime()
{
	if (timeStart == -1) return 0;
	return GetTickCount64() - timeStart;
}