#include "Character.h"
#include "PlayScene.h"
#include "Mario.h"

void CCharacter::Touched()
{
	CPlayScene* scene = (CPlayScene*)(CGame::GetInstance()->GetCurrentScene());
	CMario* mario = dynamic_cast<CMario*>(scene->GetPlayer());
	mario->Attacked();
}
