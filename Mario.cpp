#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Koopa.h"
#include "Coin.h"
#include "Portal.h"
#include "Plant.h"
#include "Fireball.h"
#include "Collision.h"
#include "QuestionBlock.h"
#include "Mushroom.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vy += ay * dt;
	vx += ax * dt;
	if (abs(vx) > abs(maxVx)) vx = maxVx;
	if (vy > 0 && abs(vy) > abs(maxVy)) vy = maxVy;
	//DebugOutTitle(L"vx: %f, vy: %f\n", vx, vy);

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);
	if (holdingShell != NULL) {
		HoldingProcess(dt, coObjects);
	}

	//Attack
	if (attackTimer > 0)
	{
		attackTimer -= dt;
		TailAttack(dt, coObjects);
	}

	if (glideTimer > 0) {
		glideTimer -= dt;
	}
	else {
		ay = MARIO_GRAVITY;
	}

	if (flyTimer > 0) {
		flyTimer -= dt;
	}
	else {
		//ay = MARIO_GRAVITY;
	}
	
}

void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
	isOnPlatform = false;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0) isOnPlatform = true;
	}
	else
		if (e->nx != 0 && e->obj->IsBlocking())
		{
			vx = 0;
		}

	if (dynamic_cast<CCharacter*>(e->obj))
		OnCollisionWithCharacter(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
	else if (dynamic_cast<CPlant*>(e->obj))
		OnCollisionWithPlant(e);
	else if (dynamic_cast<CFireball*>(e->obj))
		OnCollisionWithFireball(e);
	/*else if (dynamic_cast<CQuestionBlock*>(e->obj))
		OnCollisionWithQuestionBlock(e);*/
	else if (dynamic_cast<CMushroom*>(e->obj))
		OnCollisionWithMushroom(e);
}

void CMario::OnCollisionWithCharacter(LPCOLLISIONEVENT e)
{
	CCharacter* character = dynamic_cast<CCharacter*>(e->obj);

	// jump on top >> kill Koopa and deflect a bit 
	if (e->ny < 0)
	{
		character->Stomped();
		vy = -MARIO_JUMP_DEFLECT_SPEED;
	}
	else
	{
		if (character->CanHold() && canHold)
		{
			CKoopa* koopa = dynamic_cast<CKoopa*>(character);
			holdingShell = koopa;
			koopa->Held();
			DebugOut(L"HOLD\n");
		}else if (untouchable == 0)
		{
			character->Touched();
		}
		
	}
}

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	coin++;
}

void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}

void CMario::OnCollisionWithPlant(LPCOLLISIONEVENT e)
{
	Attacked();
}

void CMario::OnCollisionWithFireball(LPCOLLISIONEVENT e)
{	
	Attacked();
}

//void CMario::OnCollisionWithQuestionBlock(LPCOLLISIONEVENT e)
//{
//	CQuestionBlock* questionblock = (CQuestionBlock*)e->obj;
//	if (e->ny > 0 && (x >= questionblock->ReturnXmin() || x <= questionblock->ReturnXmax()))
//	{
//		if (questionblock->GetState() == QUESTION_BLOCK_STATE_ITEM)
//		{
//			questionblock->SetState(QUESTION_BLOCK_STATE_MOVEUP);
//		}
//		else
//		{
//			questionblock->SetState(QUESTION_BLOCK_STATE_ITEM);
//		}
//	}
//}

void CMario::OnCollisionWithMushroom(LPCOLLISIONEVENT e)
{
	CMushroom* mushroom = (CMushroom*)e->obj;
	mushroom->SetState(MUSHROOM_STATE_DELETE);
	if (level == MARIO_LEVEL_SMALL)
	{
		SetLevel(MARIO_LEVEL_BIG);
	}

}

void CMario::Attacked() {
	if (!untouchable)
	{
		if (level > MARIO_LEVEL_BIG)
		{
			SetLevel(MARIO_LEVEL_BIG);
			StartUntouchable();
		}
		else if (level == MARIO_LEVEL_BIG)
		{
			SetLevel(MARIO_LEVEL_SMALL);
			StartUntouchable();
		}
		else if (level == MARIO_LEVEL_RACCOON) 
		{
			SetLevel(MARIO_LEVEL_BIG);
			StartUntouchable();
		}
		else
		{
			DebugOut(L">>> Mario DIE >>> \n");
			SetState(MARIO_STATE_DIE);
		}
	}
}

void CMario::TailAttackInit()
{
	attackTimer = ATTACK_TIME;
}

void CMario::TailAttack(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if(attackTimer <= 0) return;
	float l1, t1, r1, b1;
	float l2, t2, r2, b2;
	GetTailHitBox(l1, t1, r1, b1, l2, t2, r2, b2);
	CCollision::GetInstance()->CheckTouchCharacterForTailAttack(l1, t1, r1, b1, vx, vy, dt, coObjects);
	CCollision::GetInstance()->CheckTouchCharacterForTailAttack(l2, t2, r2, b2, vx, vy, dt, coObjects);
}

void CMario::HoldTurn()
{
	if (holdingShell == NULL) return;
	if (level == MARIO_LEVEL_BIG) currentAnimation = ID_ANI_MARIO_HOLD_FRONT;
	else if (level == MARIO_LEVEL_SMALL) currentAnimation = ID_ANI_MARIO_SMALL_HOLD_FRONT;
	else if (level == MARIO_LEVEL_RACCOON) currentAnimation = ID_ANI_MARIO_RACCOON_HOLD_FRONT;
	ResetCurrentAnimation();
}

//Change animaion when mario kick the shell
void CMario::KickedShell()
{
	if (level == MARIO_LEVEL_SMALL) {
		if (nx > 0) currentAnimation = ID_ANI_MARIO_SMALL_KICK_RIGHT;
		else currentAnimation = ID_ANI_MARIO_SMALL_KICK_LEFT;
	}
	else if (level == MARIO_LEVEL_BIG) {
		if (nx > 0) currentAnimation = ID_ANI_MARIO_KICK_RIGHT;
		else currentAnimation = ID_ANI_MARIO_KICK_LEFT;
	}
	else if (level == MARIO_LEVEL_RACCOON) {
		if (nx > 0) currentAnimation = ID_ANI_MARIO_RACCOON_KICK_RIGHT;
		else currentAnimation = ID_ANI_MARIO_RACCOON_KICK_LEFT;
	}
	ResetCurrentAnimation();
}

void CMario::SpecialPressed()
{
	if (level == MARIO_LEVEL_RACCOON) {
		if (attackTimer <= 0) TailAttackInit();
	}
}

void CMario::JumpPressed()
{
	SetState(MARIO_STATE_JUMP);
	if (!isOnPlatform && level == MARIO_LEVEL_RACCOON) {
		if (glideTimer <= 0 && abs(ax) != abs(MARIO_ACCEL_RUN_X)) {
			glideTimer = GLIDE_TIME;
			ay /= 10;
			vy = 0;
		}else if (flyTimer <= 0 && abs(ax) == abs(MARIO_ACCEL_RUN_X)) {
			flyTimer = FLY_TIME;
			vy = -MARIO_JUMP_SPEED_Y;
			if (nx > 0) currentAnimation = ID_ANI_MARIO_RACCOON_TAIL_JUMP_FLY_RIGHT;
			else currentAnimation = ID_ANI_MARIO_RACCOON_TAIL_JUMP_FLY_LEFT;
			ResetCurrentAnimation();
		}
	}
}

//
// Get animation ID for small Mario
//
int CMario::GetAniIdSmall()
{
	int aniId = -1;

	if (holdingShell != NULL) {
		if (!isOnPlatform)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_HOLD_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_HOLD_LEFT;
				aniId = ID_ANI_MARIO_SMALL_JUMP_HOLD_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_HOLD_RIGHT;
				else aniId = ID_ANI_MARIO_SMALL_IDLE_HOLD_LEFT;
			}
			else if (vx > 0)
			{
				aniId = ID_ANI_MARIO_SMALL_WALK_HOLD_RIGHT;
			}
			else // vx < 0
			{
				aniId = ID_ANI_MARIO_SMALL_WALK_HOLD_LEFT;
			}
		return aniId;
	}

	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;

	return aniId;
}

int CMario::GetAniIdRaccoon()
{
	int aniId = -1;

	if (holdingShell != NULL) {
		if (!isOnPlatform)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_RACCOON_JUMP_HOLD_RIGHT;
			else
				aniId = ID_ANI_MARIO_RACCOON_JUMP_HOLD_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_RACCOON_IDLE_HOLD_RIGHT;
				else aniId = ID_ANI_MARIO_RACCOON_IDLE_HOLD_LEFT;
			}
			else if (vx > 0)
			{
				aniId = ID_ANI_MARIO_RACCOON_WALK_HOLD_RIGHT;
			}
			else // vx < 0
			{
				aniId = ID_ANI_MARIO_RACCOON_WALK_HOLD_LEFT;
			}
		return aniId;
	}

	if (attackTimer > 0) {
		if(nx > 0)
			aniId = ID_ANI_MARIO_RACCOON_TAIL_ATTACK_RIGHT;
		else
			aniId = ID_ANI_MARIO_RACCOON_TAIL_ATTACK_LEFT;
	}
	else if (!isOnPlatform && glideTimer > 0) {
		if (nx > 0)
			aniId = ID_ANI_MARIO_RACCOON_TAIL_JUMP_GLIDE_RIGHT;
		else
			aniId = ID_ANI_MARIO_RACCOON_TAIL_JUMP_GLIDE_LEFT;
	}
	else if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_RACCOON_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_RACCOON_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_RACCOON_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_RACCOON_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_RACCOON_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_RACCOON_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_RACCOON_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_RACCOON_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_RACCOON_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RACCOON_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_RACCOON_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_RACCOON_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RACCOON_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_RACCOON_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_RACCOON_IDLE_RIGHT;

	return aniId;

}


//
// Get animdation ID for big Mario
//
int CMario::GetAniIdBig()
{
	int aniId = -1;
	if (holdingShell != NULL) {
		if (!isOnPlatform)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_HOLD_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_HOLD_LEFT;
		}
		else
				if (vx == 0)
				{
					if (nx > 0) aniId = ID_ANI_MARIO_IDLE_HOLD_RIGHT;
					else aniId = ID_ANI_MARIO_IDLE_HOLD_LEFT;
				}
				else if (vx > 0)
				{
					aniId = ID_ANI_MARIO_WALK_HOLD_RIGHT;
				}
				else // vx < 0
				{
					aniId = ID_ANI_MARIO_WALK_HOLD_LEFT;
				}
		return aniId;
	}

	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT;

	return aniId;
}

void CMario::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;
		// if animation havent finished (for special animation )
	if (currentAnimation <= 0 || animations->Get(currentAnimation)->IsDone())
	{
		if (state == MARIO_STATE_DIE)
			aniId = ID_ANI_MARIO_DIE;
		else if (level == MARIO_LEVEL_BIG)
			aniId = GetAniIdBig();
		else if (level == MARIO_LEVEL_RACCOON)
			aniId = GetAniIdRaccoon();
		else if (level == MARIO_LEVEL_SMALL)
			aniId = GetAniIdSmall();
		currentAnimation = aniId;
	}

	animations->Get(currentAnimation)->Render(x, y);

	//RenderBoundingBox();
	
	//DebugOutTitle(L"Coins: %d", coin);
}

void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return; 

	// run then walk mean release

	switch (state)
	{
	case MARIO_STATE_RUNNING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		if (isSitting) break;
		if (isOnPlatform)
		{
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				vy = -MARIO_JUMP_RUN_SPEED_Y;
			else
				vy = -MARIO_JUMP_SPEED_Y;
		}
		break;

	case MARIO_STATE_RELEASE_JUMP:
		if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 2;
		break;

	case MARIO_STATE_SIT:
		if (isOnPlatform && level != MARIO_LEVEL_SMALL)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;
			y +=MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;
			y -= MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_IDLE:
		ax = 0.0f;
		vx = 0.0f;
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		vx = 0;
		ax = 0;
		break;
	}

	CGameObject::SetState(state);
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (level==MARIO_LEVEL_BIG || level == MARIO_LEVEL_RACCOON)
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else 
		{
			left = x - MARIO_BIG_BBOX_WIDTH/2;
			top = y - MARIO_BIG_BBOX_HEIGHT/2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else
	{
		left = x - MARIO_SMALL_BBOX_WIDTH/2;
		top = y - MARIO_SMALL_BBOX_HEIGHT/2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
}

void CMario::GetTailHitBox(float& l1, float& t1, float& r1, float& b1, float& l2, float& t2, float& r2, float& b2)
{
	// 1 is left hit box
	// 2 is right hit box
	if(level == MARIO_LEVEL_RACCOON)
	{
		l1 = x - 3/2 * MARIO_BIG_BBOX_WIDTH;
		t1 = y;
		r1 = l1 + MARIO_BIG_BBOX_WIDTH;
		b1 = t1 + MARIO_BIG_BBOX_HEIGHT / 2;

		l2 = x + MARIO_BIG_BBOX_WIDTH / 2;
		t2 = y;
		r2 = l2 + MARIO_BIG_BBOX_WIDTH;
		b2 = t2 + MARIO_BIG_BBOX_HEIGHT / 2;
	}
	else
	{
		l1 = l2 = t1 = t2 = r1 = r2 = b1 = b2 = 0;
	}
}

void CMario::HoldingProcess(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float hx, hy;
	//holdingShell->SetPosition(x, y);
	holdingShell->GetPosition(hx, hy);
	holdingShell->SetPositionY(y);

	// move the shell, also move faster when mario turn
	if (nx == 1)
		holdingShell->SetSpeed(min((x + KOOPA_BBOX_WIDTH - hx)/dt, MARIO_SHELL_TURNING_SPEED), vy);
	else
		holdingShell->SetSpeed(max((x - KOOPA_BBOX_WIDTH - hx) / dt, -MARIO_SHELL_TURNING_SPEED), vy);
	if (!canHold)
	{
		holdingShell->Kicked();
		KickedShell();
		holdingShell->ThrownInBlock(dt, coObjects);
		holdingShell = NULL;
	}
}

void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	if (this->level == MARIO_LEVEL_SMALL)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
	}
	level = l;
}

