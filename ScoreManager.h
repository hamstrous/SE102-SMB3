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
public:
	static CScoreManager* __instance;

	static CScoreManager* GetInstance();

	void AddScore(float x, float y, int type);
};

