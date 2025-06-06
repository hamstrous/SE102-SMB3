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
	if (type >= 100 || type == 0)
	{
		CScore* score = new CScore(x, y, type);
		scene->AddObject(score);
	}
	
	if(type < 10000 || type >=10) CGameData::GetInstance()->AddScore(type);
}

void CScoreManager::AddScoreDouble(float x, float y, int count)
{	
	LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	if (count <= 7)
	{
		AddScore(x, y, score[count]);
	}
	else
	{
		AddScore(x, y, SCORE_1UP);
		CGameData::GetInstance()->AddLife();
	}	
	
}
