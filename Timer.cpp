#include "Timer.h"

#include "TimerManager.h"


CTimer::CTimer(ULONGLONG timeSpan)
{
	this->timeSpan = timeSpan;
	timeStart = -1;
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
	if (elapsed <= timeSpan)
		return true;
	else
	{
		started = false;
		timeStart = -1;
	}
	return false;
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