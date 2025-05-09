#include "SampleKeyEventHandler.h"

#include "debug.h"
#include "Game.h"

#include "Mario.h"
#include "Character.h"
#include "PlayScene.h"
#include "Fireball.h"
#include "GameFXManager.h"

void CSampleKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CMario* mario = (CMario *)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer(); 

	switch (KeyCode)
	{
	case DIK_A:
		mario->SetCanHold(true);
		mario->SpecialPressed();
		break;
	case DIK_DOWN:
		if(!mario->canHold) mario->SetState(MARIO_STATE_SIT);
		break;
	case DIK_S:
		mario->JumpPressed();
		break;
	case DIK_1:
		mario->SetLevel(MarioLevel::SMALL);
		break;
	case DIK_2:
		mario->SetLevel(MarioLevel::BIG);
		break;
	case DIK_3:
		mario->SetLevel(MarioLevel::RACCOON);
		break;
	case DIK_0:
		mario->SetState(MARIO_STATE_DIE);
		break;
	case DIK_R: // reset
		CGame::GetInstance()->ResetCurrentScene();
		break;
	case DIK_LEFT:
		mario->StartHoldTurn(-1);
		break;
	case DIK_RIGHT:
		mario->StartHoldTurn(1);
		break;
	case DIK_W:
		dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->SetIsPause();
		break;
	}
}

void CSampleKeyHandler::OnKeyUp(int KeyCode)
{
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if(mario == NULL) return;
	switch (KeyCode)
	{
	case DIK_S:
		mario->SetState(MARIO_STATE_RELEASE_JUMP);
		mario->SetJumpInput(0);
		break;
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT_RELEASE);
	case DIK_A:
		mario->SetCanHold(false);
		mario->SetRunInput(0);
		break;
	}
}

void CSampleKeyHandler::KeyState(BYTE *states)
{
	LPGAME game = CGame::GetInstance();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if(mario == NULL) return;
	if (game->IsKeyDown(DIK_RIGHT))
	{
		mario->SetDirInput(1);
		if (game->IsKeyDown(DIK_A)) {
			mario->SetState(MARIO_STATE_RUNNING_RIGHT);
		}
		else
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		mario->SetDirInput(-1);
		if (game->IsKeyDown(DIK_A))
			mario->SetState(MARIO_STATE_RUNNING_LEFT);
		else
			mario->SetState(MARIO_STATE_WALKING_LEFT);
	}
	else {
		mario->SetState(MARIO_STATE_IDLE);
		mario->SetDirInput(0);
	}
	
}