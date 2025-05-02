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
#include "Leaf.h"


std::unordered_map<MarioLevel, std::unordered_map<MarioAnimationType, int>> animationMap = {
	{
		MarioLevel::BIG, {
			{MarioAnimationType::IDLE_RIGHT, 400},
			{MarioAnimationType::IDLE_LEFT, 401},
			{MarioAnimationType::WALKING_RIGHT, 500},
			{MarioAnimationType::WALKING_LEFT, 501},
			{MarioAnimationType::RUNNING_RIGHT, 600},
			{MarioAnimationType::RUNNING_LEFT, 601},
			{MarioAnimationType::JUMP_WALK_RIGHT, 700},
			{MarioAnimationType::JUMP_WALK_LEFT, 701},
			{MarioAnimationType::JUMP_RUN_RIGHT, 800},
			{MarioAnimationType::JUMP_RUN_LEFT, 801},
			{MarioAnimationType::SIT_RIGHT, 900},
			{MarioAnimationType::SIT_LEFT, 901},
			{MarioAnimationType::BRACE_RIGHT, 1001},
			{MarioAnimationType::BRACE_LEFT, 1000},
			{MarioAnimationType::KICK_RIGHT, 1010},
			{MarioAnimationType::KICK_LEFT, 1011},
			{MarioAnimationType::IDLE_HOLD_RIGHT, 1012},
			{MarioAnimationType::IDLE_HOLD_LEFT, 1013},
			{MarioAnimationType::WALK_HOLD_RIGHT, 1014},
			{MarioAnimationType::WALK_HOLD_LEFT, 1015},
			{MarioAnimationType::HOLD_FRONT, 1020},
			{MarioAnimationType::JUMP_HOLD_RIGHT, 1021},
			{MarioAnimationType::JUMP_HOLD_LEFT, 1022},
			{MarioAnimationType::DIE, 999}
		}
	},
	{
		MarioLevel::SMALL, {
			{MarioAnimationType::IDLE_RIGHT, 1100},
			{MarioAnimationType::IDLE_LEFT, 1102},
			{MarioAnimationType::WALKING_RIGHT, 1200},
			{MarioAnimationType::WALKING_LEFT, 1201},
			{MarioAnimationType::RUNNING_RIGHT, 1300},
			{MarioAnimationType::RUNNING_LEFT, 1301},
			{MarioAnimationType::BRACE_RIGHT, 1401},
			{MarioAnimationType::BRACE_LEFT, 1400},
			{MarioAnimationType::JUMP_WALK_RIGHT, 1500},
			{MarioAnimationType::JUMP_WALK_LEFT, 1501},
			{MarioAnimationType::JUMP_RUN_RIGHT, 1600},
			{MarioAnimationType::JUMP_RUN_LEFT, 1601},
			{MarioAnimationType::KICK_RIGHT, 1610},
			{MarioAnimationType::KICK_LEFT, 1611},
			{MarioAnimationType::IDLE_HOLD_RIGHT, 1650},
			{MarioAnimationType::IDLE_HOLD_LEFT, 1651},
			{MarioAnimationType::WALK_HOLD_RIGHT, 1652},
			{MarioAnimationType::WALK_HOLD_LEFT, 1653},
			{MarioAnimationType::HOLD_FRONT, 1654},
			{MarioAnimationType::JUMP_HOLD_RIGHT, 1655},
			{MarioAnimationType::JUMP_HOLD_LEFT, 1656},
			{MarioAnimationType::DIE, 999}
		}
	},
	{
		MarioLevel::RACCOON, {
			{MarioAnimationType::IDLE_RIGHT, 1700},
			{MarioAnimationType::IDLE_LEFT, 1702},
			{MarioAnimationType::WALKING_RIGHT, 1800},
			{MarioAnimationType::WALKING_LEFT, 1801},
			{MarioAnimationType::RUNNING_RIGHT, 1900},
			{MarioAnimationType::RUNNING_LEFT, 1901},
			{MarioAnimationType::BRACE_RIGHT, 2001},
			{MarioAnimationType::BRACE_LEFT, 2000},
			{MarioAnimationType::JUMP_WALK_RIGHT, 2100},
			{MarioAnimationType::JUMP_WALK_LEFT, 2101},
			{MarioAnimationType::JUMP_RUN_RIGHT, 2200},
			{MarioAnimationType::JUMP_RUN_LEFT, 2201},
			{MarioAnimationType::SIT_RIGHT, 2300},
			{MarioAnimationType::SIT_LEFT, 2301},
			{MarioAnimationType::TAIL_ATTACK_RIGHT, 2400},
			{MarioAnimationType::TAIL_ATTACK_LEFT, 2401},
			{MarioAnimationType::TAIL_JUMP_GLIDE_RIGHT, 2500},
			{MarioAnimationType::TAIL_JUMP_GLIDE_LEFT, 2501},
			{MarioAnimationType::TAIL_JUMP_FLY_RIGHT, 2600},
			{MarioAnimationType::TAIL_JUMP_FLY_LEFT, 2601},
			{MarioAnimationType::KICK_RIGHT, 2610},
			{MarioAnimationType::KICK_LEFT, 2611},
			{MarioAnimationType::IDLE_HOLD_RIGHT, 2650},
			{MarioAnimationType::IDLE_HOLD_LEFT, 2651},
			{MarioAnimationType::WALK_HOLD_RIGHT, 2652},
			{MarioAnimationType::WALK_HOLD_LEFT, 2653},
			{MarioAnimationType::HOLD_FRONT, 2654},
			{MarioAnimationType::JUMP_HOLD_RIGHT, 2655},
			{MarioAnimationType::JUMP_HOLD_LEFT, 2656},
			{MarioAnimationType::DIE, 999}
		}
	}
};



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
	else if (dynamic_cast<CBaseBrick*>(e->obj))
		OnCollisionWithBaseBrick(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
	else if (dynamic_cast<CPlant*>(e->obj))
		OnCollisionWithPlant(e);
	else if (dynamic_cast<CFireball*>(e->obj))
		OnCollisionWithFireball(e);
	else if (dynamic_cast<CMushroom*>(e->obj))
		OnCollisionWithMushroom(e);
	else if (dynamic_cast<CLeaf*>(e->obj))
		OnCollisionWithLeaf(e);
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

void CMario::OnCollisionWithBaseBrick(LPCOLLISIONEVENT e)
{
	CBaseBrick* brick = dynamic_cast<CBaseBrick*>(e->obj);
	if (e->ny > 0)
	{
		brick->BottomHit();
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

void CMario::OnCollisionWithMushroom(LPCOLLISIONEVENT e)
{
	CMushroom* mushroom = (CMushroom*)e->obj;
	mushroom->SetState(MUSHROOM_STATE_DELETE);
	if (level == MarioLevel::SMALL)
	{
		SetLevel(MarioLevel::BIG);
	}
}

void CMario::OnCollisionWithLeaf(LPCOLLISIONEVENT e)
{	
	e->obj->Delete();
	SetLevel(MarioLevel::RACCOON);
}

void CMario::Attacked() {
	if (!untouchable)
	{
		if (level == MarioLevel::RACCOON)
		{
			SetLevel(MarioLevel::BIG);
			StartUntouchable();
		}
		else if (level == MarioLevel::BIG)
		{
			SetLevel(MarioLevel::SMALL);
			StartUntouchable();
		}
		else if (level > MarioLevel::BIG) // For future levels beyond RACCOON
		{
			SetLevel(MarioLevel::BIG);
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
	AssignCurrentAnimation(level, nx > 0 ? MarioAnimationType::TAIL_ATTACK_RIGHT : MarioAnimationType::TAIL_ATTACK_LEFT);
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

void CMario::HoldTurn(int dir)
{
	if (holdingShell == NULL) return;
	if (nx != dir)
		AssignCurrentAnimation(level,MarioAnimationType::HOLD_FRONT);
}

//Change animaion when mario kick the shell
void CMario::KickedShell()
{
	AssignCurrentAnimation(level, nx > 0 ? MarioAnimationType::KICK_RIGHT : MarioAnimationType::KICK_LEFT);
}

void CMario::SpecialPressed()
{
	if (level == MarioLevel::RACCOON) {
		if (attackTimer <= 0) TailAttackInit();
	}
}

void CMario::JumpPressed()
{
	SetState(MARIO_STATE_JUMP);
	if (!isOnPlatform && level == MarioLevel::RACCOON) {
		if (glideTimer <= 0 && abs(ax) != abs(MARIO_ACCEL_RUN_X)) {
			glideTimer = GLIDE_TIME;
			AssignCurrentAnimation(level, nx > 0 ? MarioAnimationType::TAIL_JUMP_GLIDE_RIGHT : MarioAnimationType::TAIL_JUMP_GLIDE_LEFT);
			ay /= 10;
			vy = 0;
		}else if (flyTimer <= 0 && abs(ax) == abs(MARIO_ACCEL_RUN_X)) {
			flyTimer = FLY_TIME;
			vy = -MARIO_JUMP_SPEED_Y;
			AssignCurrentAnimation(level, nx > 0 ? MarioAnimationType::TAIL_JUMP_FLY_RIGHT : MarioAnimationType::TAIL_JUMP_FLY_LEFT);
		}
	}
}

void CMario::GetAniId()
{
	if (state == MARIO_STATE_DIE)
	{
		currentAnimation = animationMap[level][MarioAnimationType::DIE];
		return;
	}

	if (holdingShell != NULL) {
		if (!isOnPlatform) {
			currentAnimation = animationMap[level][nx >= 0 ? MarioAnimationType::JUMP_HOLD_RIGHT : MarioAnimationType::JUMP_HOLD_LEFT];
		}
		else if (vx == 0) {
			currentAnimation = animationMap[level][nx > 0 ? MarioAnimationType::IDLE_HOLD_RIGHT : MarioAnimationType::IDLE_HOLD_LEFT];
		}
		else if (vx > 0) {
			currentAnimation = animationMap[level][MarioAnimationType::WALK_HOLD_RIGHT];
		}
		else { // vx < 0
			currentAnimation = animationMap[level][MarioAnimationType::WALK_HOLD_LEFT];
		}
		return;
	}

	if (!isOnPlatform) {
		if (abs(ax) == MARIO_ACCEL_RUN_X) {
			currentAnimation = animationMap[level][nx >= 0 ? MarioAnimationType::JUMP_RUN_RIGHT : MarioAnimationType::JUMP_RUN_LEFT];
		}
		else {
			currentAnimation = animationMap[level][nx >= 0 ? MarioAnimationType::JUMP_WALK_RIGHT : MarioAnimationType::JUMP_WALK_LEFT];
		}
	}
	else if (isSitting) {
		currentAnimation = animationMap[level][nx > 0 ? MarioAnimationType::SIT_RIGHT : MarioAnimationType::SIT_LEFT];
	}
	else if (vx == 0) {
		currentAnimation = animationMap[level][nx > 0 ? MarioAnimationType::IDLE_RIGHT : MarioAnimationType::IDLE_LEFT];
	}
	else if (vx > 0) {
		if (ax < 0) {
			currentAnimation = animationMap[level][MarioAnimationType::BRACE_RIGHT];
		}
		else if (ax == MARIO_ACCEL_RUN_X) {
			currentAnimation = animationMap[level][MarioAnimationType::RUNNING_RIGHT];
		}
		else if (ax == MARIO_ACCEL_WALK_X) {
			currentAnimation = animationMap[level][MarioAnimationType::WALKING_RIGHT];
		}
	}
	else { // vx < 0
		if (ax > 0) {
			currentAnimation = animationMap[level][MarioAnimationType::BRACE_LEFT];
		}
		else if (ax == -MARIO_ACCEL_RUN_X) {
			currentAnimation = animationMap[level][MarioAnimationType::RUNNING_LEFT];
		}
		else if (ax == -MARIO_ACCEL_WALK_X) {
			currentAnimation = animationMap[level][MarioAnimationType::WALKING_LEFT];
		}
	}

	if (currentAnimation == -1) {
		currentAnimation = animationMap[level][MarioAnimationType::IDLE_RIGHT];
	}
}

void CMario::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
		// if animation havent finished (for special animation )
	if (currentAnimation <= 0 || animations->Get(currentAnimation)->IsDone())
	{
		GetAniId();
	}

	if(untouchable)
	{
		animations->Get(currentAnimation)->SetType(2);
	}
	else {
		animations->Get(currentAnimation)->SetType(0);
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
		if (isOnPlatform && level != MarioLevel::SMALL)
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
	if (level==MarioLevel::BIG || level == MarioLevel::RACCOON)
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
	if(level == MarioLevel::RACCOON)
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

void CMario::SetLevel(MarioLevel l)
{
	// Adjust position to avoid falling off platform
	if (level == MarioLevel::SMALL)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
	}
	level = l;
}

