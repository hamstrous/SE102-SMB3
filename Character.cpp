#include "Character.h"
#include "Koopa.h"
#include "PlayScene.h"
#include "Mario.h"
#include "Smoke.h"
#include "GameData.h"
#include "ScoreManager.h"

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
	CScoreManager::GetInstance()->AddScore(x, y, SCORE_1000);
	isDeleted = true;
}

void CCharacter::OnOverlapWith(LPCOLLISIONEVENT e)
{
	{
		if (CKoopa *koopa = dynamic_cast<CKoopa*>(e->obj))
		{
			if (koopa->IsMoving()) ShellHit(koopa->GetX());
		}
	}
}
