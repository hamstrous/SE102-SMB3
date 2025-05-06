#pragma once
#include "Timer.h"
#include "Mario.h"
#include <vector>

using namespace std;

class CGameFXManager
{
public:
	static CGameFXManager* __instance;

	static CGameFXManager* GetInstance();
};

