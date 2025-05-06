#include "ScoreManager.h"

CScoreManager* CScoreManager::__instance = NULL;

CScoreManager* CScoreManager::GetInstance()
{
	if (__instance == NULL) __instance = new CScoreManager();
	return __instance;
}

void CScoreManager::AddScore(float x, float y, int type)
{
	LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	CScore* score = new CScore(x, y, type);
	scene->AddObject(score);
	DebugOut(L"[INFO] Add Score: %d\n", type);
}
