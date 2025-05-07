#include "GameFXManager.h"
#include "debug.h"
CGameFXManager* CGameFXManager::__instance = NULL;

CGameFXManager* CGameFXManager::GetInstance()
{
	if (__instance == NULL) __instance = new CGameFXManager();
	return __instance;
}

void CGameFXManager::AddGameFX(float x, float y, int type)
{
	LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	CGameFX* fx = new CGameFX(x, y, type);
	scene->AddObject(fx);
	DebugOut(L"[INFO] Add GameFX: %d\n", type);
}

void CGameFXManager::AddGamePause(float x, float y, int type)
{
	LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	CGameFX* fx = new CGameFX(0, 0, TYPE_GAMEPAUSE);
	scene->AddObject(fx);
}

void CGameFXManager::InitPauseFX()
{
	if (pauseFX == nullptr) {
		pauseFX = new CGameFX(0,0,0);
	}
	else {
		pauseFX->SetOn(true);
	}
}

void CGameFXManager::AddPause()
{
	if (pauseFX) pauseFX->SetOn(true);
}

void CGameFXManager::RemovePause()
{
	if (pauseFX) pauseFX->SetOn(false);
}
