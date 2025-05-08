#pragma once
#include "Timer.h"
#include "Mario.h"
#include <vector>
#include "Game.h"
#include "PlayScene.h"
#include "GameFX.h"
#include "GameObject.h"

using namespace std;

class CGameFXManager
{
public:
	static CGameFXManager* __instance;

	static CGameFXManager* GetInstance();

	CGameFX* pauseFX = nullptr;

	void AddGameFX(float x, float y, int type);
	//void AddGamePause(float x, float y, int type);
	void InitPauseFX();
	void AddPause();
	void RemovePause();
};

