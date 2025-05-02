#pragma once
#include "GameObject.h"

class CTimer
{
private:
	ULONGLONG timeStart;
	ULONGLONG timeSpan;
	bool started;
public:
	CTimer(ULONGLONG timeSpan);
	CTimer();

	void SetTimeSpan(ULONGLONG timeSpan) { this->timeSpan = timeSpan; }
	bool IsStarted() { return this->started; }
	void Start();
	bool IsRunning();
	void Reset();
	void Flip();
	ULONGLONG ElapsedTime();
};

