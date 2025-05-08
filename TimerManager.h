#pragma once
#include "Timer.h"

class CTimerManager
{
	static CTimerManager* __instance;
	vector<CTimer*> timers;
public:
	void Pause() {
		for (auto i : timers) {
			i->Pause();
		}
	}

	void Resume() {
		for (auto i : timers) {
			i->Resume();
		}
	}

	void Clear() {
		timers.clear();
	}

	void Add(CTimer* timer) {
		timers.push_back(timer);
	}

	static CTimerManager* GetInstance() {
		if (__instance == NULL) __instance = new CTimerManager();
		return __instance;
	}
};

