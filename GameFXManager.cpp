#include "GameFXManager.h"
#include "GameFXBreak.h"
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
}

void CGameFXManager::AddTimeup(float x, float y, int type)
{		
	CGame* game = CGame::GetInstance();
	float hx = game->GetBackBufferWidth() / 2;
	float hy = game->GetBackBufferHeight() + 700;
	LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	CGameFX* fx = new CGameFX(hx, hy, TYPE_TIMEUP);
	scene->AddObject(fx);
}

void CGameFXManager::AddBreak(float x, float y)
{
	CGameFXBreak* br = new CGameFXBreak(x, y, TYPE_LEFT_BOT_BREAK);
	CGameFXBreak* br2 = new CGameFXBreak(x, y, TYPE_LEFT_TOP_BREAK);
	CGameFXBreak* br3 = new CGameFXBreak(x, y, TYPE_RIGHT_BOT_BREAK);
	CGameFXBreak* br4 = new CGameFXBreak(x, y, TYPE_RIGHT_TOP_BREAK);
	LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	scene->AddObject(br);
	scene->AddObject(br2);
	scene->AddObject(br3);
	scene->AddObject(br4);
}

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

void CGameFXManager::InitTimeupFX()
{	
	CGame* game = CGame::GetInstance();
	float hx = game->GetBackBufferWidth() / 2;
	float hy = game->GetBackBufferHeight() - HUD_SIZE_Y;
	if (timeupFX == nullptr)
	{
		timeupFX = new CGameFX(hx, hy, TYPE_TIMEUP);
		LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
		scene->AddObject(timeupFX);
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
