#pragma once
#include "Timer.h"
#include "Mario.h"
#include <vector>
#include "debug.h"

#define FRAME_24 400
#define FRAME_16 267
#define FRAME_8 133
#define FRAME_255 4250

using namespace std;

class CGameData
{
public:
	static CGameData* __instance;
	int life;
	int pmeter;
	int score;
	int coin;
	CTimer* countDown;
	CTimer* f24;
	CTimer* f16;
	CTimer* f8;
	CTimer* f255;
	CTimer* ptimer;
	int worldName;
	bool flightMode = 0;
	bool isPaused = 0;
	ULONGLONG pauseStart = 0;
	vector<int> cards;
	MarioLevel marioLevel;

	CGameData() : life(3), pmeter(0), score(0), worldName(1), coin(0) {
		f24 = new CTimer(FRAME_24);
		f16 = new CTimer(FRAME_16);
		f8 = new CTimer(FRAME_8);
		f255 = new CTimer(FRAME_255);
		ptimer = f8;
		countDown = new CTimer(300);
		countDown->Start();
	}
	void Reset()
	{
		life = 4;
		pmeter = 0;
		score = 0;
		worldName = 1;
		cards.clear();
	}

	void OnDeath();

	void NextLevel()
	{
		countDown->Reset();
		pmeter = 0;
	}

	int GetRemainingTime();

	void Update(DWORD dt);

	void AddScore(int s)
	{	
		DebugOut(L"AddScore is called");
		score += s;
	}

	void AddCoin(int c)
	{
		coin += c;
	}

	void AddLife(int l = 1)
	{
		life += l;
	}

	bool IsPMeterFull()
	{
		return pmeter == 7;
	}

	bool IsFlightMode()
	{
		return flightMode;
	}

	static CGameData* GetInstance();
};

