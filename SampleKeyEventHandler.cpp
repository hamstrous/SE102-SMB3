#include "SampleKeyEventHandler.h"

#include "debug.h"
#include "Game.h"

#include "Mario.h"
#include "Character.h"
#include "PlayScene.h"
#include "Fireball.h"
#include "GameFXManager.h"
#include "Utils.h"

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
		//if (mario->ReturnDownPress()) break;
		////if(!mario->GetHolding()) mario->SetState(MARIO_STATE_SIT);
		//mario->SetPressDown();
		break;
	case DIK_UP:
		if (mario->ReturnRenderMarioInPipe()) break;
		mario->SetPressUp();
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
	case DIK_SPACE:
		debug = true;
		mario->SetState(MARIO_STATE_DEBUG);
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
		mario->ReleasePress();
		break;
	case DIK_UP:
		mario->ReleasePress();
		break;
	case DIK_A:
		mario->SetCanHold(false);
		mario->SetRunInput(0);
		break;
	case DIK_SPACE:
		debug = false;
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
		if (game->IsKeyDown(DIK_A)) {
			mario->SetState(MARIO_STATE_RUNNING_RIGHT);
		}
		else
			mario->SetState(MARIO_STATE_WALKING_RIGHT);

		mario->ReleasePress();
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (game->IsKeyDown(DIK_A))
			mario->SetState(MARIO_STATE_RUNNING_LEFT);
		else
			mario->SetState(MARIO_STATE_WALKING_LEFT);

		mario->ReleasePress();
	}
	else {
		mario->SetState(MARIO_STATE_IDLE);
	}
	
	if (mario->GetState() == MARIO_STATE_DEBUG) {
		if (game->IsKeyDown(DIK_S)) {
			mario->SetState(MARIO_STATE_JUMP);
		}
		else 
			if (game->IsKeyDown(DIK_DOWN)) {
				mario->SetState(MARIO_STATE_SIT);
			}
	}

	if (game->IsKeyDown(DIK_DOWN)) {
		if (!mario->GetHolding() && !mario->IsSitting() && !mario->ReturnRenderMarioInPipe()) mario->SetState(MARIO_STATE_SIT);
		mario->SetPressDown();
		
	}
}