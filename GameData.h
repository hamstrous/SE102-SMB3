#pragma once
#include "Timer.h"
#include "Mario.h"
#include <vector>
#include "debug.h"

#define FRAME_24 400
#define FRAME_16 267
#define FRAME_8 133
#define FRAME_255 4250

#define SCORE_REDUCE_SPEED 0.1f

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
	int fixedRemainingTime = -1;
	bool timeToScore = false;
	CTimer* f24;
	CTimer* f16;
	CTimer* f8;
	CTimer* f255;
	CTimer* ptimer;
	int worldName;
	bool flightMode = 0;
	bool isPaused = 0;
	ULONGLONG pauseStart = 0;
	ULONGLONG levelTime = 300;
	int justWonCard = -1;
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
		marioLevel = MarioLevel::SMALL;
	}

	~CGameData() {
		if (f24 != NULL) delete f24;
		if (f16 != NULL) delete f16;
		if (f8 != NULL) delete f8;
		if (f255 != NULL) delete f255;
		if (countDown != NULL) delete countDown;
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
	void OnWin();

	void NextLevel()
	{
		countDown->Reset();
		pmeter = 0;
	}

	int GetRemainingTime();

	void Update(DWORD dt);

	void AddScore(int s)
	{	
		//DebugOut(L"AddScore is called");
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

	void SetJustWonCard(int card)
	{
		justWonCard = card;
		fixedRemainingTime = GetRemainingTime();
	}

	void StartTimeToScore() 
	{
		timeToScore = true;
	}

	static CGameData* GetInstance();
};

