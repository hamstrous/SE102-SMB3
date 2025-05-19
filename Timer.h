#pragma once
#include "GameObject.h"

class CTimer
{
private:
	ULONGLONG timeStart;
	ULONGLONG pauseStart = 0;
	ULONGLONG timeSpan;
	bool started;

public:
	CTimer(ULONGLONG timeSpan);
	CTimer();

	void SetTimeSpan(ULONGLONG timeSpan) { this->timeSpan = timeSpan; }
	bool IsStarted() { return this->started; }
	void Start();
	bool IsRunning();
	bool IsDone();
	void Reset();
	void Flip();
	void Pause();
	void Resume();
	void AddToManager();

	ULONGLONG GetTimeStart() { return timeStart; }
	ULONGLONG ElapsedTime();
	ULONGLONG RemainingTime();
};

