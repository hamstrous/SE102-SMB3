#pragma once
#include "Timer.h"
#include "Mario.h"
#include <vector>

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
	int worldName;
	vector<int> cards;
	MarioLevel marioLevel;

	CGameData() : life(3), pmeter(0), score(0), worldName(1), coin(0) {
		countDown = new CTimer(300);
		countDown->Start();
	}
	void Reset()
	{
		life = 3;
		pmeter = 0;
		score = 0;
		worldName = 1;
		cards.clear();
	}

	void NextLevel()
	{
		countDown->Reset();
		pmeter = 0;
	}

	int GetRemainingTime()
	{
		return countDown->ElapsedTime() / 1000;
	}

	static CGameData* GetInstance();
};

