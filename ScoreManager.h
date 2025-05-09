#pragma once
#include "Timer.h"
#include "Mario.h"
#include <vector>
#include "Game.h"
#include "PlayScene.h"
#include "Score.h"
#include "GameObject.h"
#include "GameData.h"

using namespace std;

class CScoreManager
{
protected:
	vector<int> score = { 100,200,400,800,1000,2000,4000,8000 };
public:
	static CScoreManager* __instance;

	static CScoreManager* GetInstance();

	void AddScore(float x, float y, int type);
	void AddScoreDouble(float x, float y, int count);
	void Clear();
};

