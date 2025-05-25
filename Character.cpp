#include "Character.h"
#include "PlayScene.h"
#include "Mario.h"
#include "Smoke.h"
#include "GameData.h"

void CCharacter::Touched()
{
	CPlayScene* scene = (CPlayScene*)(CGame::GetInstance()->GetCurrentScene());
	CMario* mario = dynamic_cast<CMario*>(scene->GetPlayer());
	mario->Attacked();
}

void CCharacter::Ending()
{
	CSmoke* smoke = new CSmoke(x, y);
	((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->AddObject(smoke);
	CGameData::GetInstance()->AddScore(1000);
	isDeleted = true;
}
