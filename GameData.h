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
	CTimer countDown;
	int worldName;
	vector<int> cards;
	MarioLevel marioLevel;

	CGameData() : life(3), pmeter(0), score(0), worldName(1) {}
	static CGameData* GetInstance();
};

