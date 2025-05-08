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
#include "Font.h"
#include "ScoreManager.h"


unordered_map<MarioLevel, unordered_map<MarioAnimationType, int>> CMario::animationMap = {
	{
		MarioLevel::BIG, {
			{MarioAnimationType::IDLE_RIGHT, 400},
			{MarioAnimationType::IDLE_LEFT, 401},
			{MarioAnimationType::WALKING_RIGHT, 500},
			{MarioAnimationType::WALKING_LEFT, 501},
			{MarioAnimationType::RUNNING_RIGHT, 600}, //fix da ktra
			{MarioAnimationType::RUNNING_LEFT, 601},
			{MarioAnimationType::SPRINTING_RIGHT, 602}, //fix da ktra
			{MarioAnimationType::SPRINTING_LEFT, 603},
			{MarioAnimationType::JUMP_RIGHT, 700},
			{MarioAnimationType::JUMP_LEFT, 701},
			{MarioAnimationType::JUMP_FALL_RIGHT,702}, // da them va ktra
			{MarioAnimationType::JUMP_FALL_LEFT,703}, // da them va ktra
			{MarioAnimationType::JUMP_SPRINT_RIGHT, 800},
			{MarioAnimationType::JUMP_SPRINT_LEFT, 801},
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
			{MarioAnimationType::GROW_RIGHT, 1023},
			{MarioAnimationType::GROW_LEFT, 1024},
			{MarioAnimationType::SHRINK_RIGHT, 1025},
			{MarioAnimationType::SHRINK_LEFT, 1026},
			{MarioAnimationType::RUN_HOLD_RIGHT, 1016}, //fix da ktra
			{MarioAnimationType::RUN_HOLD_LEFT, 1017},
			{MarioAnimationType::SPRINT_HOLD_RIGHT, 1018}, //fix da ktra
			{MarioAnimationType::SPRINT_HOLD_LEFT, 1019},
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
			{MarioAnimationType::SPRINTING_RIGHT, 1302},
			{MarioAnimationType::SPRINTING_LEFT, 1303},
			{MarioAnimationType::BRACE_RIGHT, 1401},
			{MarioAnimationType::BRACE_LEFT, 1400},
			{MarioAnimationType::JUMP_RIGHT, 1500},
			{MarioAnimationType::JUMP_LEFT, 1501},
			{MarioAnimationType::JUMP_SPRINT_RIGHT, 1600},
			{MarioAnimationType::JUMP_SPRINT_LEFT, 1601},
			{MarioAnimationType::KICK_RIGHT, 1610},
			{MarioAnimationType::KICK_LEFT, 1611},
			{MarioAnimationType::IDLE_HOLD_RIGHT, 1650},
			{MarioAnimationType::IDLE_HOLD_LEFT, 1651},
			{MarioAnimationType::WALK_HOLD_RIGHT, 1652},
			{MarioAnimationType::WALK_HOLD_LEFT, 1653},
			{MarioAnimationType::HOLD_FRONT, 1654},
			{MarioAnimationType::JUMP_HOLD_RIGHT, 1655},
			{MarioAnimationType::JUMP_HOLD_LEFT, 1656},
			{MarioAnimationType::GROW_RIGHT, 1657},
			{MarioAnimationType::GROW_LEFT, 1658},
			{MarioAnimationType::RUN_HOLD_RIGHT, 1660}, //fix da kiem tra
			{MarioAnimationType::RUN_HOLD_LEFT, 1661},
			{MarioAnimationType::SPRINT_HOLD_RIGHT, 1662}, //fix da kiemtra
			{MarioAnimationType::SPRINT_HOLD_LEFT, 1663},
			{MarioAnimationType::DIE, 999}
		}
	},
	{
		MarioLevel::RACCOON, {
			{MarioAnimationType::IDLE_RIGHT, 1700},
			{MarioAnimationType::IDLE_LEFT, 1702},
			{MarioAnimationType::WALKING_RIGHT, 1800},
			{MarioAnimationType::WALKING_LEFT, 1801},
			{MarioAnimationType::RUNNING_RIGHT, 1900}, //fix da ktra
			{MarioAnimationType::RUNNING_LEFT, 1901},
			{MarioAnimationType::SPRINTING_RIGHT, 1902}, //fix da ktra
			{MarioAnimationType::SPRINTING_LEFT, 1903},
			{MarioAnimationType::BRACE_RIGHT, 2001},
			{MarioAnimationType::BRACE_LEFT, 2000},
			{MarioAnimationType::JUMP_RIGHT, 2100},
			{MarioAnimationType::JUMP_LEFT, 2101},
			{MarioAnimationType::JUMP_FALL_RIGHT,1908}, // da them va ktra
			{MarioAnimationType::JUMP_FALL_LEFT,1909}, // da them va ktra
			{MarioAnimationType::JUMP_SPRINT_RIGHT, 2200},
			{MarioAnimationType::JUMP_SPRINT_LEFT, 2201},
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
			{MarioAnimationType::SHRINK_RIGHT, 1023},
			{MarioAnimationType::SHRINK_LEFT, 1024},
			{MarioAnimationType::RUN_HOLD_RIGHT, 1904}, //fix da ktra
			{MarioAnimationType::RUN_HOLD_LEFT, 1905},
			{MarioAnimationType::SPRINT_HOLD_RIGHT, 1906}, //fix da ktra
			{MarioAnimationType::SPRINT_HOLD_LEFT, 1907},
			{MarioAnimationType::DIE, 999}
		}
	}
};



void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	Acceleration(dt);

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// for mario has to be called first so process can call OnCollision
	CCollision::GetInstance()->Process(this, dt, coObjects);
	if (holdingShell != NULL) {
		HoldingProcess(dt, coObjects);
	}

	//Attack
	if (attackTimer->IsRunning())
	{
		TailAttack(dt, coObjects);
	}
}

void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
	isOnPlatform = false;
	PointsCheck();
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	float objx, objy;
	e->obj->GetPosition(objx, objy);
	if (e->obj->IsBlocking()) {
		PointsCheck();
		if (e->ny != 0)
		{
			if (e->ny > 0) {
				// head collide offset
				if (pointsTouched[TOP]) vy = 0;
				else y -= 1; //let mario phase through block so side points can do it job of offseting
			}else if (e->ny < 0) {
				count = 0;
				if (!pointsTouched[DOWNRIGHT] && !pointsTouched[DOWNLEFT]) {
					y += 1;
					isOnPlatform = false;
				}
				else {
					vy = 0;
					isOnPlatform = true;
					if (glideTimer->IsRunning()) {
						glideTimer->Reset();
						SkipCurrentAnimation();
					}
				}
			}
		}
		else if (e->nx != 0)
		{
			vx = 0;
		}

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
	float characterX, characterY;
	character->GetPosition(characterX, characterY);
	// jump on top >> kill Koopa and deflect a bit 
	if (e->ny < 0)
	{	
		if (dynamic_cast<CPlant*>(character)) {
			character->Touched();
			return;
		}
		character->Stomped();
		if(count <= 7) CScoreManager::GetInstance()->AddScore(characterX, characterY, score[count]);
		else CScoreManager::GetInstance()->AddScore(characterX, characterY, score[7]);
		count++;

		if (CGame::GetInstance()->IsKeyDown(DIK_S)) SetJumpInput(1);
		if (jumpInput == 1) vy = -MARIO_JUMP_DEFLECT_SPEED;
		else vy = -MARIO_JUMP_WEAK_DEFLECT_SPEED;
	}
	else if (e->ny > 0)
	{
		character->Touched();
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
	CGameData::GetInstance()->AddCoin(1);
	
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
	e->obj->SetState(LEAF_STATE_DELETE);
	SetLevel((MarioLevel)((int)level + 1));
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
	attackTimer->Start();
	AssignCurrentAnimation(level, nx > 0 ? MarioAnimationType::TAIL_ATTACK_RIGHT : MarioAnimationType::TAIL_ATTACK_LEFT);
}

void CMario::TailAttack(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{	
	//CScoreManager::GetInstance()->AddScore(characterX, characterY, score[count]);
	//count++;
	if(!attackTimer->IsRunning()) return;
	float l1, t1, r1, b1;
	float l2, t2, r2, b2;
	GetTailHitBox(l1, t1, r1, b1, l2, t2, r2, b2);
	CCollision::GetInstance()->CheckTouchCharacterForTailAttack(l1, t1, r1, b1, 0, 0, dt, coObjects, x, -nx, y);
	CCollision::GetInstance()->CheckTouchCharacterForTailAttack(l2, t2, r2, b2, 0, 0, dt, coObjects, x, nx, y);
}

//Change animaion when mario kick the shell
void CMario::KickedShell()
{
	AssignCurrentAnimation(level, nx > 0 ? MarioAnimationType::KICK_RIGHT : MarioAnimationType::KICK_LEFT);
}

void CMario::SpecialPressed()
{
	SetRunInput(1);
	if (level == MarioLevel::RACCOON) {
		if (!attackTimer->IsRunning()) TailAttackInit();
	}
}

void CMario::JumpPressed()
{
	SetState(MARIO_STATE_JUMP);
	if (!isOnPlatform && level == MarioLevel::RACCOON) {
		if (!IsPMeterFull() && vy >= 0) {
			glideTimer->Start();
			AssignCurrentAnimation(level, nx > 0 ? MarioAnimationType::TAIL_JUMP_GLIDE_RIGHT : MarioAnimationType::TAIL_JUMP_GLIDE_LEFT);
			vy = MARIO_RACCOON_GLIDE_SPEED;
		}else if(IsPMeterFull()){
			flyTimer->Start();
			vy = -MARIO_RACCOON_FLY_SPEED;
			AssignCurrentAnimation(level, nx > 0 ? MarioAnimationType::TAIL_JUMP_FLY_RIGHT : MarioAnimationType::TAIL_JUMP_FLY_LEFT);
		}
	}else(SetJumpInput(1));
	
}

bool CMario::IsPMeterFull()
{
	CGameData* gameData = CGameData::GetInstance();
	return gameData->IsPMeterFull();
}

#define POINTS_OFFSET 0.1f

void CMario::SetPointsPosition()
{
	if (!IsBig() || isSitting) {
		const float MARIO_SMALL_Y_OFFSET = 7.0f;
		const float MARIO_SMALL_X_OFFSET = 3.0f;
		points[0]->SetPosition(x, y - MARIO_SMALL_BBOX_HEIGHT / 2 - POINTS_OFFSET);
		points[1]->SetPosition(x + MARIO_SMALL_BBOX_WIDTH / 2 - POINTS_OFFSET, y - MARIO_SMALL_Y_OFFSET);
		points[2]->SetPosition(x + MARIO_SMALL_BBOX_WIDTH / 2 - POINTS_OFFSET, y + MARIO_SMALL_Y_OFFSET);
		points[3]->SetPosition(x + MARIO_SMALL_X_OFFSET, y + MARIO_SMALL_BBOX_HEIGHT / 2 + POINTS_OFFSET);
		points[4]->SetPosition(x - MARIO_SMALL_X_OFFSET, y + MARIO_SMALL_BBOX_HEIGHT / 2 + POINTS_OFFSET);
		points[5]->SetPosition(x - MARIO_SMALL_BBOX_WIDTH / 2 + POINTS_OFFSET, y + MARIO_SMALL_Y_OFFSET);
		points[6]->SetPosition(x - MARIO_SMALL_BBOX_WIDTH / 2 + POINTS_OFFSET, y - MARIO_SMALL_Y_OFFSET);
	}
	else {
		const float MARIO_BIG_Y_OFFSET = 12.0f;
		const float MARIO_BIG_X_OFFSET = 4.0f;
		points[0]->SetPosition(x, y - MARIO_BIG_BBOX_HEIGHT / 2 - POINTS_OFFSET);
		points[1]->SetPosition(x + MARIO_BIG_BBOX_WIDTH/2 - POINTS_OFFSET, y - MARIO_BIG_Y_OFFSET);
		points[2]->SetPosition(x + MARIO_BIG_BBOX_WIDTH/2 - POINTS_OFFSET, y + MARIO_BIG_Y_OFFSET);
		points[3]->SetPosition(x + MARIO_BIG_X_OFFSET, y + MARIO_BIG_BBOX_HEIGHT/2 + POINTS_OFFSET);
		points[4]->SetPosition(x - MARIO_BIG_X_OFFSET, y + MARIO_BIG_BBOX_HEIGHT/2 + POINTS_OFFSET);
		points[5]->SetPosition(x - MARIO_BIG_BBOX_WIDTH / 2 + POINTS_OFFSET, y + MARIO_BIG_Y_OFFSET);
		points[6]->SetPosition(x - MARIO_BIG_BBOX_WIDTH / 2 + POINTS_OFFSET, y - MARIO_BIG_Y_OFFSET);
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
			return;
		}

		if (vx < 0 && ax > 0 || vx > 0 && ax < 0) {
			currentAnimation = animationMap[level][MarioAnimationType::HOLD_FRONT];
			return;
		}

		if (vx == 0) {
			currentAnimation = animationMap[level][nx > 0 ? MarioAnimationType::IDLE_HOLD_RIGHT : MarioAnimationType::IDLE_HOLD_LEFT];
		}
		else if (vx > 0) {
			if(vx <= MARIO_WALK_MAX_SPEED_X) currentAnimation = animationMap[level][MarioAnimationType::WALK_HOLD_RIGHT];
			else if (vx <= MARIO_RUN_MAX_SPEED_X) currentAnimation = animationMap[level][MarioAnimationType::RUN_HOLD_RIGHT];
			else if (vx <= MARIO_SPRINT_MAX_SPEED_X) currentAnimation = animationMap[level][MarioAnimationType::SPRINT_HOLD_RIGHT];
		}
		else { // vx < 0
			if (abs(vx) <= MARIO_WALK_MAX_SPEED_X) currentAnimation = animationMap[level][MarioAnimationType::WALK_HOLD_LEFT];
			else if (abs(vx) <= MARIO_RUN_MAX_SPEED_X) currentAnimation = animationMap[level][MarioAnimationType::RUN_HOLD_LEFT];
			else if (abs(vx) <= MARIO_SPRINT_MAX_SPEED_X) currentAnimation = animationMap[level][MarioAnimationType::SPRINT_HOLD_LEFT];
		}
		return;
	}

	if (!isOnPlatform) {
		if (IsPMeterFull()) {
			currentAnimation = animationMap[level][nx >= 0 ? MarioAnimationType::JUMP_SPRINT_RIGHT : MarioAnimationType::JUMP_SPRINT_LEFT];
		}
		else {
			// run and walk
			currentAnimation = animationMap[level][nx >= 0 ? MarioAnimationType::JUMP_RIGHT : MarioAnimationType::JUMP_LEFT];
			if(jumpInput == 0 && level != MarioLevel::SMALL)
				currentAnimation = animationMap[level][nx >= 0 ? MarioAnimationType::JUMP_FALL_RIGHT : MarioAnimationType::JUMP_FALL_LEFT];
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
		else if (vx <= MARIO_WALK_MAX_SPEED_X) {
			currentAnimation = animationMap[level][MarioAnimationType::WALKING_RIGHT];
		}
		else if (vx <= MARIO_RUN_MAX_SPEED_X) {
			currentAnimation = animationMap[level][MarioAnimationType::RUNNING_RIGHT];
		}
		else {
			currentAnimation = animationMap[level][MarioAnimationType::SPRINTING_RIGHT];
		}
		
	}
	else { // vx < 0
		if (ax > 0) {
			currentAnimation = animationMap[level][MarioAnimationType::BRACE_LEFT];
		}
		else if (abs(vx) <= MARIO_WALK_MAX_SPEED_X) {
			currentAnimation = animationMap[level][MarioAnimationType::WALKING_LEFT];
		}
		else if (abs(vx) <= MARIO_RUN_MAX_SPEED_X) {
			currentAnimation = animationMap[level][MarioAnimationType::RUNNING_LEFT];
		}
		else {
			currentAnimation = animationMap[level][MarioAnimationType::SPRINTING_LEFT];
		}
	}

	if (currentAnimation == -1) {
		currentAnimation = animationMap[level][MarioAnimationType::IDLE_RIGHT];
	}
}

float num = 0;

void CMario::Render()
{
	if (GetIsPause()) return;
	CAnimations* animations = CAnimations::GetInstance();
		// if animation havent finished (for special animation )
	if (currentAnimation <= 0 || animations->Get(currentAnimation)->IsDone())
	{
		GetAniId();
	}

	if(untouchable)
	{
		// if stopping animation then flicker and stopping 
		if(animations->Get(currentAnimation)->GetType() == 3) 
			animations->Get(currentAnimation)->SetType(4);
		else 
			animations->Get(currentAnimation)->SetType(2);
	}
	else {
		animations->Get(currentAnimation)->ResetType();
	}

	animations->Get(currentAnimation)->Render(x, y);

	RenderBoundingBox();
	
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
		dirInput = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		dirInput = -1;
		break;
	case MARIO_STATE_JUMP:
		if (isSitting) break;
		if (isOnPlatform)
		{
			vy = 0;
			for (int i = 0; i < 3; i++) {
				if (abs(vx) < MARIO_JUMP_SPEED_CHECK_X[i]) {
					vy = -MARIO_JUMP_SPEED[i];
					break;
				}
			}
			if (vy == 0) vy = -MARIO_JUMP_SPEED[3];
			jumpVx = vx;
			break;
		}
		jumpVx = 0;
		break;

	case MARIO_STATE_RELEASE_JUMP:
		jumpInput = 0;
		//if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 2;
		break;

	case MARIO_STATE_SIT:
		if (isOnPlatform && level != MarioLevel::SMALL)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vy = 0.0f;
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
		dirInput = 0;
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_SPEED_Y;
		CPlayScene* s = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		s->OnPlayerDie();
		vx = 0;
		ax = 0;
		break;
	}

	CGameObject::SetState(state);
}

void CMario::PointsCheck()
{
	if (state == MARIO_STATE_DIE) return;
	SetPointsPosition();
	vector<LPGAMEOBJECT> coObjects;
	GetCollidableObjects(&coObjects);
	CCollision::GetInstance()->ProcessForMario(this, &points, &coObjects, &pointsTouched);

	for (int i = 0; i < 7; i++)
	{
		float px, py;
		points[i]->GetPosition(px, py);
		if(!pointsTouched[i]) points[i]->SetOldPosition(px, py);
	}

	int dir = 0;
	if (pointsTouched[LEFTUP] || pointsTouched[LEFTDOWN]) {
		dir = 2;
	}
	else if (pointsTouched[RIGHTUP] || pointsTouched[RIGHTDOWN]) {
		dir = -2;
	}

	x += dir;
	if ((vx < 0 && dir > 0) || (vx > 0 && dir < 0)) {
		vx = 0;
	}
}

void CMario::Acceleration(DWORD dt)
{
	CGameData* gameData = CGameData::GetInstance();

	float topSpeed = 0;

	if (IsPMeterFull()) topSpeed = MARIO_SPRINT_MAX_SPEED_X;
	else if (runInput == 1) topSpeed = MARIO_RUN_MAX_SPEED_X;
	else topSpeed = MARIO_WALK_MAX_SPEED_X;

	//if(vy < 0) minY  = min(minY, y);
	//DebugOutTitle(L"vx: %f", vx);
	if (state == MARIO_STATE_DIE) {
		if (vy < -0.12) {
			vy += MARIO_GRAVITY_SLOW * dt;
		}
		else {
			vy += MARIO_GRAVITY_FAST * dt;  // Normal gravity
		}
		return;
	}

	if (dirInput == 0) {
		if (isOnPlatform) {
			if (vx < 0) {
				vx += (IsBig() ? MARIO_BIG_ACCEL_FRIC_X : MARIO_SMALL_ACCEL_FRIC_X) * dt;
				if (vx > 0) {
					vx = 0;
				}
			}
			else if (vx > 0) {
				vx -= (IsBig() ? MARIO_BIG_ACCEL_FRIC_X : MARIO_SMALL_ACCEL_FRIC_X) * dt;
				if (vx < 0) {
					vx = 0;
				}
			}
		}
		else if (IsRaccoon()) {
			if (abs(vx) > MARIO_RACCOON_MIDAIR_SPEED_LIMIT) {
				vx += (vx > 0 ? -MARIO_RACCOON_MIDAIR_DECEL : MARIO_RACCOON_MIDAIR_DECEL) * dt;
			}
		}
	}
	else {
		//dirInput != 0;
		const float absVx = abs(vx);
		if (isOnPlatform) {
			if (
				(vx > 0 && dirInput < 0) ||
				(vx < 0 && dirInput > 0)
				)
			{
				// Skidding (turning around)
				vx += dirInput * MARIO_ACCEL_SKID_X * dt;
			}
			else if (absVx < abs(topSpeed)) {
				// Normal acceleration
				vx += dirInput * MARIO_ACCEL_NORMAL_X * dt;
				if(abs(vx) > topSpeed)	vx = topSpeed * dirInput;
			}
			else if (absVx > abs(topSpeed)) {
				if (isOnPlatform) {
					vx -= dirInput * (IsBig() ? MARIO_BIG_ACCEL_FRIC_X : MARIO_SMALL_ACCEL_FRIC_X) * dt;
				}
			}
		}
		else {
			// midair
			if (!IsPMeterFull()) {
				if(abs(jumpVx) <= MARIO_WALK_MAX_SPEED_X) topSpeed = MARIO_WALK_MAX_SPEED_X;
				else topSpeed = MARIO_RUN_MAX_SPEED_X;
			}else topSpeed = MARIO_SPRINT_MAX_SPEED_X;

			if (!IsRaccoon()) {
				if ((vx > 0 && dirInput < 0) || (vx < 0 && dirInput > 0)) {
					vx += dirInput * MARIO_DECEL_MIDAIR_X * dt;
				}
				else if (absVx < abs(topSpeed)) {
					vx += dirInput * MARIO_ACCEL_MIDAIR_X * dt;
				}
				else if (absVx > abs(topSpeed)) {
					vx = topSpeed * dirInput;
				}
			}
			else {
				const float NEW_MARIO_RACCOON_MIDAIR_DECEL = 0.00005625f;
				const float NEW_MARIO_RACCOON_MIDAIR_DECEL_OPPOSITE = 0.000675f; //0030
				if (gameData->IsFlightMode()) {
					if (vx < 0 && dirInput> 0 || vx > 0 && dirInput < 0) {
						vx += dirInput * NEW_MARIO_RACCOON_MIDAIR_DECEL_OPPOSITE * dt;
					}
					else if (abs(vx) < MARIO_RACCOON_MIDAIR_SPEED_LIMIT) {
						vx += dirInput * MARIO_ACCEL_MIDAIR_X * dt;
					}else
						vx += (vx > 0 ? -NEW_MARIO_RACCOON_MIDAIR_DECEL : NEW_MARIO_RACCOON_MIDAIR_DECEL) * dt;

				}
				else {
					if (vx < 0 && dirInput> 0 || vx > 0 && dirInput < 0) {
						vx += dirInput * NEW_MARIO_RACCOON_MIDAIR_DECEL_OPPOSITE * dt;
					}
					else {
						vx += dirInput * MARIO_ACCEL_MIDAIR_X * dt;
					}

					if (abs(vx) > topSpeed) {
						vx = topSpeed * dirInput;

					}
					DebugOutTitle(L"vx %f", vx);
				}
			}
		}
	}

	if(glideTimer->IsRunning() || flyTimer->IsRunning())
	{

	}else if (vy < -0.12 && jumpInput == 1) {
		vy += MARIO_GRAVITY_SLOW * dt;
	}
	else {
		vy += MARIO_GRAVITY_FAST * dt;  // Normal gravity
	}

	vy = min(vy, MARIO_MAX_FALL_SPEED_Y);

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
		l1 = x - 0.7 * MARIO_BIG_BBOX_WIDTH;
		t1 = y;
		r1 = l1 + MARIO_BIG_BBOX_WIDTH;
		b1 = t1 + MARIO_BIG_BBOX_HEIGHT / 2;

		l2 = x + MARIO_BIG_BBOX_WIDTH / 2;
		t2 = y;
		r2 = l2 + MARIO_BIG_BBOX_WIDTH * 0.4;
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
	if(l > MarioLevel::RACCOON)
	{
		l = MarioLevel::RACCOON;
	}
	// Adjust position to avoid falling off platform
	if (level == MarioLevel::SMALL)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
	}
	if((int)l - (int)level == 1)
	{
		AssignCurrentAnimation(level, nx > 0 ? MarioAnimationType::GROW_RIGHT : MarioAnimationType::GROW_LEFT);
	}else if((int)l - (int)level == -1)
	{
		AssignCurrentAnimation(level, nx > 0 ? MarioAnimationType::SHRINK_RIGHT : MarioAnimationType::SHRINK_LEFT);
	}
	level = l;
}

