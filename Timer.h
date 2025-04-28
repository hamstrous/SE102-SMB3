#pragma once
#include "GameObject.h"

class Timer
{
private:
	ULONGLONG timeStart;
	ULONGLONG timeSpan;
	bool started;
public:
	Timer(ULONGLONG timeSpan);
	Timer();

	void SetTimeSpan(ULONGLONG timeSpan) { this->timeSpan = timeSpan; }
	bool IsStarted() { return this->started; }
	void Start();
	bool IsDone();
	void Reset();
	ULONGLONG ElapsedTime();
};

