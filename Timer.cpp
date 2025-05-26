#include "Timer.h"

#include "TimerManager.h"

// timeStart == -1 <=> time start = largest value so comparision with -1 after overflow still work
CTimer::CTimer(ULONGLONG timeSpan)
{
	this->timeSpan = timeSpan;
	timeStart = -1; // overflow to largest value
	started = false;
	AddToManager();
}

CTimer::CTimer()
{
	timeSpan = 0;
	timeStart = -1;
	started = false;
	AddToManager();
}

void CTimer::Start()
{
	timeStart = GetTickCount64();
	started = true;
}

bool CTimer::IsRunning()
{
	if (timeStart == -1) return false;
	if (timeSpan == -1) return true;
	ULONGLONG elapsed = GetTickCount64() - timeStart;
	if (elapsed <= timeSpan) return true;
	else return false;
}

bool CTimer::IsDone()
{
	if(timeSpan == -1) return false;
	if (timeStart == -1) return false;
	if(ElapsedTime() >= timeSpan)
		return true;
	return false;
}

void CTimer::Reset()
{
	timeStart = -1;
	started = false;
}

void CTimer::Flip()
{
	if(!IsRunning())
	{
		Start();
	}
	else
	{
		Reset();
	}
}

void CTimer::Pause()
{
	pauseStart = GetTickCount64();
}

void CTimer::Resume()
{
	if (timeStart == -1) return;
	ULONGLONG elapsed = GetTickCount64() - pauseStart;
	timeStart += elapsed;
	started = true;
}

void CTimer::AddToManager()
{
	CTimerManager::GetInstance()->Add(this);
}

ULONGLONG CTimer::ElapsedTime()
{
	if (timeStart == -1) return 0;
	return GetTickCount64() - timeStart;
}

ULONGLONG CTimer::RemainingTime()
{
	if (timeStart == -1) return MAXULONGLONG;
	if (timeSpan == -1) return MAXULONGLONG;
	ULONGLONG elapsed = GetTickCount64() - timeStart;
	if (elapsed >= timeSpan) return 0;
	return timeSpan - elapsed;
}
