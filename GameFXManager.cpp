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

//void CGameFXManager::AddGamePause(float x, float y, int type)
//{	
//	CGame* game = CGame::GetInstance();
//	float hx = game->GetBackBufferWidth() / 2;
//	float hy = game->GetBackBufferHeight() - HUD_SIZE_Y;
//	LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
//	CGameFX* fx = new CGameFX(hx, hy, TYPE_GAMEPAUSE);
//	scene->AddObject(fx);
//}

void CGameFXManager::InitPauseFX()
{	
	CGame* game = CGame::GetInstance();
	float hx = game->GetBackBufferWidth() / 2;
	float hy = game->GetBackBufferHeight() - HUD_SIZE_Y;
	if (pauseFX == nullptr) {
		pauseFX = new CGameFX(hx, hy, TYPE_GAMEPAUSE);
		LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
		scene->AddObject(pauseFX);
	}
	else {
		pauseFX->SetOn(true);
	}
}

void CGameFXManager::AddPause()
{	
	if (pauseFX) pauseFX->SetOn(false);
}

void CGameFXManager::RemovePause()
{
	if (pauseFX) pauseFX->SetOn(true);
}
