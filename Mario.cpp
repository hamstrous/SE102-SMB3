#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Koopa.h"
#include "KoopaGreen.h"
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
#include "GameFXManager.h"
#include "Boomerang.h"
#include "Abyss.h"
#include "Utils.h"
#include "Pipe.h"
#include "MovingPlatform.h"
#include "Switch.h"
#include "Camera.h"
#include "ColorBlock.h"
#include "Mountain.h"
#include "Decoration.h"
#include "Cloud.h"
#include "BreakableBrick.h"

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

void CMario::GoingPipe(DWORD dt) {
	CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
	CCamera* camera = scene->GetCamera();
	SetState(MARIO_STATE_GOIN_PIPE);
	if (goDownPipe)
	{
		if (!pipeFadeoutStarted && y >= distancePipeGo - 25) {
			scene->SetTimeFadeoutAlphaPipe();
			pipeFadeoutStarted = true;
		}
		y += SPEED_IN_PIPE * dt;
		if (y >= distancePipeGo )
		{
			goDownPipe = false;
			if (typePipe % 2 == 0) outUpPipe = true;
			else outDownPipe = true;
			distancePipeOut = typePipe % 2 ? PipeLocation[typePipe].second + 38 : PipeLocation[typePipe].second - 31;
			if (level != MarioLevel::SMALL)
				distancePipeOut -= 6;
			scene->SetPlayerPosition(PipeLocation[typePipe].first, PipeLocation[typePipe].second);
			pipeFadeoutStarted = false;
		}
		
	}
	if (goUpPipe)
	{
		if (!pipeFadeoutStarted && y <= distancePipeGo + 15) {
			scene->SetTimeFadeoutAlphaPipe();
			pipeFadeoutStarted = true;
		}
		y -= SPEED_IN_PIPE * dt;
		if (y <= distancePipeGo)
		{
			goUpPipe = false;
			if (typePipe % 2 == 0) outUpPipe = true;
			else outDownPipe = true;
			distancePipeOut = typePipe % 2 ? PipeLocation[typePipe].second + 38 : PipeLocation[typePipe].second - 31;
			if (level != MarioLevel::SMALL)
				distancePipeOut -= 6;
			scene->SetPlayerPosition(PipeLocation[typePipe].first, PipeLocation[typePipe].second);
			pipeFadeoutStarted = false;
		}
	}
	if (outDownPipe)
	{	
		if (typePipe == 1) camera->SetState(CAMERA_STATE_SECRET_ROOM); 
		y += SPEED_IN_PIPE * dt;
		if (y >= distancePipeOut)
		{
			outDownPipe = false;
			renderMarioInPipe = false;
			pipeFadeoutStarted = false;
		}
	}
	if (outUpPipe)
	{	
		if (typePipe == 0) camera->SetState(CAMERA_STATE_FOLLOW);
		else if (typePipe == 2) camera->SetState(CAMERA_STATE_1_4_END);
		y -= SPEED_IN_PIPE * dt;
		if (y <= distancePipeOut)
		{
			outUpPipe = false;
			renderMarioInPipe = false;
			pipeFadeoutStarted = false;
		}
	}
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if(state == MARIO_STATE_DEBUG) return;

	if (renderMarioInPipe) {
		GoingPipe(dt);
		return;
	}
	else canSit = false;

	if (isOnMovingFlatform) {
		y += SPEED_Y_MOVING_PLATFORM * dt;
		isOnMovingFlatform = false;
	}
	Acceleration(dt);

	if (pointsDisable > 0) pointsDisable--;
	isBehind = false;

	//DebugOutTitle(L"l: %f, vt: %f, r: %f, b: %f\n", l, t, r, b);

	// reset untouchable timer if untouchable time has passed
	// for mario has to be called first so process can call OnCollision
	SetPointsPosition();
	//CCollision::GetInstance()->ProcessCollision(this, dt, coObjects);
	CCollision::GetInstance()->ProcessOverlap(this, dt, coObjects);
	CCollision::GetInstance()->ProcessMarioPoints(this, &points, coObjects, dt);

	if (!isBehind && hideTimer->ElapsedTime() >= HIDE_TIME) {
		hideTimer->Reset();
	}

	if (holdingShell != NULL) {
		HoldingProcess(dt, coObjects);
	}

	//Attack
	if (attackTimer->IsRunning())
	{
		TailAttack(dt, coObjects);
	}

	if (untouchableTimer->IsDone()) {
		untouchableTimer->Reset();
	}
}

void CMario::OnNoCollision(DWORD dt)
{
	if(GetIsStop()) return;
	x += vx * dt;
	y += vy * dt;
	isOnPlatform = false;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	float objx, objy;
	e->obj->GetPosition(objx, objy);
	if (e->obj->IsBlocking()) {
		if (e->ny != 0)
		{	
			if (e->ny > 0) {
				vy = 0;
			}else if (e->ny < 0) {
				count = 0;
				vy = 0;
				isOnPlatform = true;
				if (state == MARIO_STATE_WIN) vx = MARIO_WIN_WALKING_SPEED;
				if (glideTimer->IsRunning()) {
					glideTimer->Reset();
					SkipCurrentAnimation();
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
	else if (dynamic_cast<CBreakableBrick*>(e->obj))
		OnCollisionWithBreakableBrick(e);
	else if (dynamic_cast<CBaseBrick*>(e->obj))
		OnCollisionWithBaseBrick(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
	else if (dynamic_cast<CFireball*>(e->obj))
		OnCollisionWithFireball(e);
	else if (dynamic_cast<CMushroom*>(e->obj))
		OnCollisionWithMushroom(e);
	else if (dynamic_cast<CLeaf*>(e->obj))
		OnCollisionWithLeaf(e);
	else if (dynamic_cast<CPipe*>(e->obj))
		OnCollisionWithPipe(e);
	else if (dynamic_cast<CSwitch*>(e->obj))
		OnCollisionWithSwitch(e);
	else if (dynamic_cast<CBoomerang*>(e->obj))
		OnCollisionWithBoomerang(e);
	else if (dynamic_cast<CColorBlock*>(e->obj))
		OnCollisionWithColorBlock(e);
	else if (dynamic_cast<CMovingPlatform*>(e->obj))
		OnCollisionWithMovingPlatfrom(e);
	else if (dynamic_cast<CAbyss*>(e->obj))
	{
		DebugOut(L"[INFO] abyss\n");
		SetState(MARIO_STATE_DIE);
	}
	SetSpeed(vx, vy); // reset speed after collision
}

void CMario::OnOverlapWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CColorBlock*>(e->obj)
		|| dynamic_cast<CMountain*>(e->obj)
		|| dynamic_cast<CDecoration*>(e->obj)
		|| dynamic_cast<CCloud*>(e->obj)) 
		OnOverlapWithBackGroundObjectWhenHide(e);
}

void CMario::OnCollisionWithCharacter(LPCOLLISIONEVENT e)
{
	if(isBehind) return;
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
		CScoreManager::GetInstance()->AddScoreDouble(characterX, characterY, count);
		count++;

		if (CGame::GetInstance()->IsKeyDown(DIK_S)) SetJumpInput(1);
		if (jumpInput == 1) vy = -MARIO_JUMP_DEFLECT_SPEED;
		else vy = -MARIO_JUMP_WEAK_DEFLECT_SPEED;
		glideTimer->Reset();
	}
	else if (e->ny > 0)
	{
		character->Touched();
	}
	else
	{
		if (CKoopa* koopa = dynamic_cast<CKoopa*>(character)) {
			if (koopa->CanHold() && canHold)
			{
				holdingShell = koopa;
				koopa->Held();
				return;
			}
			else if (shellProtectTimer->IsRunning()) return;
		}
		if (!untouchableTimer->IsRunning())
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

void CMario::OnCollisionWithBreakableBrick(LPCOLLISIONEVENT e)
{
	CBaseBrick* brick = dynamic_cast<CBaseBrick*>(e->obj);
	if (e->ny > 0)
	{
		brick->BottomHit();
	}

	if (!CGame::GetInstance()->GetChangeBricktoCoin()) return;
	e->obj->Delete();
	CGameData::GetInstance()->AddCoin(1);
	CGameData::GetInstance()->AddScore(50);
	
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

void CMario::OnCollisionWithFireball(LPCOLLISIONEVENT e)
{	
	Attacked();
}

void CMario::OnCollisionWithBoomerang(LPCOLLISIONEVENT e)
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

void CMario::OnCollisionWithPipe(LPCOLLISIONEVENT e)
{	
	CPipe* pipe = (CPipe*)e->obj;
	float pipeX, pipeY;
	pipe->GetPosition(pipeX, pipeY);
	tempState = state;

	if ((pipe->IsGoInside() == 1) && y < pipeY && (pipeX) < x && x < (pipeX + PIPE_RANGE)) canSit = true;
	

	if ( (pipe->IsGoInside() == 1) && y < pipeY && downPress && (pipeX) < x && x < (pipeX + PIPE_RANGE) )
	{	
		renderMarioInPipe = true;
		typePipe = pipe->GetType();
		goDownPipe = true;
		distancePipeGo = pipeY + DISTANCE_GO_DOWN_PIPE;
	}
	if ( (pipe->IsGoInside() == 2) && y > pipeY && upPress && (pipeX) < x && x < (pipeX + PIPE_RANGE))
	{
		renderMarioInPipe = true;
		typePipe = pipe->GetType();
		goUpPipe = true;
		distancePipeGo = pipeY - DISTANCE_GO_UP_PIPE;
	}


}

void CMario::OnCollisionWithSwitch(LPCOLLISIONEVENT e)
{	
	if(e->ny < 0)
	{
		CSwitch* sw = (CSwitch*)e->obj;
		sw->Enable();
	}
}

void CMario::OnCollisionWithMovingPlatfrom(LPCOLLISIONEVENT e)
{
	if (e->ny < 0) {
		isOnPlatform = true;
		isOnMovingFlatform = true;
	}
	//currentPlatform = dynamic_cast<CMovingPlatform*>(e->obj);
	//vy + 0.05;
}

void CMario::OnCollisionWithColorBlock(LPCOLLISIONEVENT e)
{
	CColorBlock* colorBlock = dynamic_cast<CColorBlock*>(e->obj);
	if (colorBlock->GetColor() != 0) return;
	if (e->ny < 0)
	{
		if (isSitting && sittingTimer->ElapsedTime() >= SIT_TIME_TO_HIDE) {
			y += 1;
			pointsDisable = 3;
			hideTimer->Start();
		}
	}
}

void CMario::OnOverlapWithBackGroundObjectWhenHide(LPCOLLISIONEVENT e)
{
	if (hideTimer->IsRunning()) isBehind = true;
}

void CMario::Attacked() {
	if(IsBehind()) return;
	if (!untouchableTimer->IsRunning())
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
	brickHit = false;
	AssignCurrentAnimation(level, nx > 0 ? MarioAnimationType::TAIL_ATTACK_RIGHT : MarioAnimationType::TAIL_ATTACK_LEFT);
}

void CMario::TailAttack(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!attackTimer->IsRunning()) return;
	float elapsed = attackTimer->ElapsedTime();
	if (elapsed >= ATTACK_TIME - COOLDOWN_TIME) return;
	float l1, t1, r1, b1;
	float l2, t2, r2, b2;
	GetTailHitBox(l1, t1, r1, b1, l2, t2, r2, b2);
	//left
	if (nx < 0 && elapsed < ATTACK_TIME / 2
		|| nx > 0 && (elapsed >= ATTACK_TIME / 2))
			CCollision::GetInstance()->CheckTouchCharacterForTailAttack(l1, t1, r1, b1, 0, 0, dt, coObjects, x, -nx, y, brickHit);

	//right
	if (nx > 0 && elapsed < ATTACK_TIME / 2
		|| nx < 0 && (elapsed >= ATTACK_TIME / 2))
			CCollision::GetInstance()->CheckTouchCharacterForTailAttack(l2, t2, r2, b2, 0, 0, dt, coObjects, x, nx, y, brickHit);
}

//Change animaion when mario kick the shell
void CMario::KickedShell(float kx)
{
	shellProtectTimer->Start();
	nx = kx > x ? 1 : -1;
	AssignCurrentAnimation(level, nx > 0 ? MarioAnimationType::KICK_RIGHT : MarioAnimationType::KICK_LEFT);
}

void CMario::SpecialPressed()
{
	if (state == MARIO_STATE_WIN || state == MARIO_STATE_DIE) return;
	SetRunInput(1);
	if (level == MarioLevel::RACCOON) {
		if (!attackTimer->IsRunning()) TailAttackInit();
	}
}

void CMario::JumpPressed()
{
	if (state == MARIO_STATE_WIN || state == MARIO_STATE_DIE) return;
	if (!isOnPlatform && level == MarioLevel::RACCOON) {
		if (!IsPMeterFull() && vy >= 0) {
			glideTimer->Start();
			AssignCurrentAnimation(level, nx > 0 ? MarioAnimationType::TAIL_JUMP_GLIDE_RIGHT : MarioAnimationType::TAIL_JUMP_GLIDE_LEFT);
			vy = MARIO_RACCOON_GLIDE_SPEED;
		}
		else if (IsPMeterFull()) {
			flyTimer->Start();
			vy = -MARIO_RACCOON_FLY_SPEED;
			AssignCurrentAnimation(level, nx > 0 ? MarioAnimationType::TAIL_JUMP_FLY_RIGHT : MarioAnimationType::TAIL_JUMP_FLY_LEFT);
		}
	}
	SetState(MARIO_STATE_JUMP);
	SetJumpInput(1);
	
}

bool CMario::IsPMeterFull()
{
	CGameData* gameData = CGameData::GetInstance();
	return gameData->IsPMeterFull();
}

#define POINTS_OFFSET 0.1f

void CMario::SetPointsPosition()
{
	const float MARIO_RIGHTLEFT_DOWN_Y_OFFSET = 6.0f;
	const float MARIO_FOOT_EXTRA_OFFSET = 3.0f;

	if (!IsBig() || isSitting) {
		const float MARIO_SMALL_Y_OFFSET = 5.0f;
		const float MARIO_SMALL_X_OFFSET = 3.0f;
		const float MARIO_SMALL_Y_UP_OFFSET = 4.0f;
		points[0]->SetPosition(x, y - MARIO_SMALL_BBOX_HEIGHT / 2.f);

		points[1]->SetPosition(x + MARIO_SMALL_BBOX_WIDTH / 2.f, y - MARIO_SMALL_Y_OFFSET);

		points[2]->SetPosition(x + MARIO_SMALL_BBOX_WIDTH / 2.f, y + MARIO_SMALL_BBOX_HEIGHT / 2 - MARIO_RIGHTLEFT_DOWN_Y_OFFSET);

		points[3]->SetPosition(x + MARIO_SMALL_X_OFFSET + (nx > 0 ? MARIO_FOOT_EXTRA_OFFSET : 0), y + MARIO_SMALL_BBOX_HEIGHT / 2.f);

		points[4]->SetPosition(x - MARIO_SMALL_X_OFFSET - (nx < 0 ? MARIO_FOOT_EXTRA_OFFSET : 0), y + MARIO_SMALL_BBOX_HEIGHT / 2.f);

		points[5]->SetPosition(x - MARIO_SMALL_BBOX_WIDTH / 2.f, y + MARIO_SMALL_BBOX_HEIGHT / 2 - MARIO_RIGHTLEFT_DOWN_Y_OFFSET);

		points[6]->SetPosition(x - MARIO_SMALL_BBOX_WIDTH / 2.f, y - MARIO_SMALL_Y_OFFSET);

	}
	else {
		const float MARIO_BIG_Y_OFFSET = 5.0f;
		const float MARIO_BIG_X_OFFSET = 3.0f;
		const float MARIO_BIG_TOPSIDE_Y_OFFSET = 5.0f;
		points[0]->SetPosition(x, y - MARIO_BIG_BBOX_HEIGHT / 2.f);

		points[1]->SetPosition(x + MARIO_BIG_BBOX_WIDTH / 2.f, y - MARIO_BIG_Y_OFFSET - MARIO_BIG_TOPSIDE_Y_OFFSET);

		points[2]->SetPosition(x + MARIO_BIG_BBOX_WIDTH / 2.f, y + MARIO_BIG_BBOX_HEIGHT/2 - MARIO_RIGHTLEFT_DOWN_Y_OFFSET );

		points[3]->SetPosition(x + MARIO_BIG_X_OFFSET + (nx > 0 ? MARIO_FOOT_EXTRA_OFFSET : 0), y + MARIO_BIG_BBOX_HEIGHT / 2.f);

		points[4]->SetPosition(x - MARIO_BIG_X_OFFSET - (nx < 0 ? MARIO_FOOT_EXTRA_OFFSET : 0), y + MARIO_BIG_BBOX_HEIGHT / 2.f);

		points[5]->SetPosition(x - MARIO_BIG_BBOX_WIDTH / 2.f, y + MARIO_BIG_BBOX_HEIGHT / 2 - MARIO_RIGHTLEFT_DOWN_Y_OFFSET);

		points[6]->SetPosition(x - MARIO_BIG_BBOX_WIDTH / 2.f, y - MARIO_BIG_Y_OFFSET - MARIO_BIG_TOPSIDE_Y_OFFSET);
	}
	SetSpeed(vx, vy);

}

void CMario::SetPointsPositionForNonBlockingOverlap()
{

	if (!IsBig() || isSitting) {
		points[0]->SetPosition(x, y - MARIO_SMALL_BBOX_HEIGHT / 2.f);

		points[1]->SetPosition(x + MARIO_SMALL_BBOX_WIDTH / 2.f, y - MARIO_SMALL_BBOX_HEIGHT / 2.f);

		points[2]->SetPosition(x + MARIO_SMALL_BBOX_WIDTH / 2.f, y);

		points[3]->SetPosition(x + MARIO_SMALL_BBOX_WIDTH / 2.f, y + MARIO_SMALL_BBOX_HEIGHT / 2.f);

		points[4]->SetPosition(x - MARIO_SMALL_BBOX_WIDTH / 2.f, y + MARIO_SMALL_BBOX_HEIGHT / 2.f);

		points[5]->SetPosition(x - MARIO_SMALL_BBOX_WIDTH / 2.f, y);

		points[6]->SetPosition(x - MARIO_SMALL_BBOX_WIDTH / 2.f, y - MARIO_SMALL_BBOX_HEIGHT / 2.f);

	}
	else {
		const float MARIO_BIG_Y_OFFSET = 3.0f;
		points[0]->SetPosition(x, y - MARIO_BIG_BBOX_HEIGHT / 2.f);

		points[1]->SetPosition(x + MARIO_BIG_BBOX_WIDTH / 2.f, y - MARIO_BIG_BBOX_HEIGHT / 2.f);

		points[2]->SetPosition(x + MARIO_BIG_BBOX_WIDTH / 2.f, y + MARIO_BIG_Y_OFFSET);

		points[3]->SetPosition(x + MARIO_BIG_BBOX_WIDTH / 2.f, y + MARIO_BIG_BBOX_HEIGHT / 2.f);

		points[4]->SetPosition(x - MARIO_BIG_BBOX_WIDTH / 2.f, y + MARIO_BIG_BBOX_HEIGHT / 2.f);

		points[5]->SetPosition(x - MARIO_BIG_BBOX_WIDTH / 2.f, y + MARIO_BIG_Y_OFFSET);

		points[6]->SetPosition(x - MARIO_BIG_BBOX_WIDTH / 2.f, y - MARIO_BIG_BBOX_HEIGHT / 2.f);
	}
	SetSpeed(vx, vy);
}

void CMario::GetAniId()
{
	if (state == -1) {
		DebugOut(L"[ERROR] Mario state is -1\n");
	}

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

		if (turnHoldTimer->IsRunning()) {
			currentAnimation = animationMap[level][MarioAnimationType::HOLD_FRONT];
			return;
		}

		if (nx != 0) {
			bool right = nx > 0;
			float absVx = abs(vx);

			MarioAnimationType animType =
				absVx == 0 ? (right ? MarioAnimationType::IDLE_HOLD_RIGHT : MarioAnimationType::IDLE_HOLD_LEFT) :
				absVx <= MARIO_WALK_MAX_SPEED_X ? (right ? MarioAnimationType::WALK_HOLD_RIGHT : MarioAnimationType::WALK_HOLD_LEFT) :
				absVx <= MARIO_RUN_MAX_SPEED_X ? (right ? MarioAnimationType::RUN_HOLD_RIGHT : MarioAnimationType::RUN_HOLD_LEFT) :
				(right ? MarioAnimationType::SPRINT_HOLD_RIGHT : MarioAnimationType::SPRINT_HOLD_LEFT);

			currentAnimation = animationMap[level][animType];
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
	else if (vx != 0) {
		bool right = vx > 0;
		bool bracing = (right && dirInput < 0) || (!right && dirInput > 0);

		currentAnimation = bracing
			? animationMap[level][right ? MarioAnimationType::BRACE_RIGHT : MarioAnimationType::BRACE_LEFT]
			: (abs(vx) <= MARIO_WALK_MAX_SPEED_X
				? animationMap[level][right ? MarioAnimationType::WALKING_RIGHT : MarioAnimationType::WALKING_LEFT]
				: (abs(vx) <= MARIO_RUN_MAX_SPEED_X
					? animationMap[level][right ? MarioAnimationType::RUNNING_RIGHT : MarioAnimationType::RUNNING_LEFT]
					: animationMap[level][right ? MarioAnimationType::SPRINTING_RIGHT : MarioAnimationType::SPRINTING_LEFT]));
	}

	if (currentAnimation == -1) {
		currentAnimation = animationMap[level][MarioAnimationType::IDLE_RIGHT];
	}
}

void CMario::GetAniIdInPipe()
{
	currentAnimation = animationMap[level][MarioAnimationType::HOLD_FRONT];
}

CMario::CMario(float x, float y) : CCharacter(x, y)
{ // set mario to game data
	glideTimer = new CTimer(GLIDE_TIME);
	flyTimer = new CTimer(FLY_TIME);
	attackTimer = new CTimer(ATTACK_TIME);
	untouchableTimer = new CTimer(UNTOUCHABLE_TIME);
	turnHoldTimer = new CTimer(TURN_TIME);
	shellProtectTimer = new CTimer(PROTECT_FROM_SHELL_TIME);
	sittingTimer = new CTimer(-1);
	hideTimer = new CTimer(-1); //-1 cause even tho hiding have finished if mario still hide then still apply
	isSitting = false;
	maxVx = 0.0f;
	maxVy = 0.3f;
	ax = 0.0f;
	ay = MARIO_GRAVITY;
	level = CGameData::GetInstance()->marioLevel;
	isOnPlatform = false;
	coin = 0;
	canHold = false;

	holdingShell = NULL;

	SetState(MARIO_STATE_IDLE);
	points.resize(7); // top, left, leftdown, downleft, dowmright, rightdown, rightup
	for (int i = 0; i < 7; i++) {
		points[i] = new CPoint(0, 0);
		float px = 0, py = 0;
		points[i]->GetPosition(px, py);
	}
}
float num = 0;

void CMario::Render()
{
	if (GetIsPause()) return;
	CAnimations* animations = CAnimations::GetInstance();
		// if animation havent finished (for special animation )
	if (currentAnimation <= 0 || animations->Get(currentAnimation)->IsDone() || IsSitting())
	{
		GetAniId();
	}

	if(untouchableTimer->IsRunning())
	{
		// when stop dont flicker too
		if(animations->Get(currentAnimation)->GetType() != 3) 
			animations->Get(currentAnimation)->SetType(2);
	}
	else {
		animations->Get(currentAnimation)->ResetType();
	}

	if (renderMarioInPipe)
	{
		GetAniIdInPipe();
	}

	animations->Get(currentAnimation)->Render(x, y);

	RenderBoundingBox();
	for(int i=0;i<7;i++)
	{
		points[i]->RenderBoundingBox();
	}
	
	//DebugOutTitle(L"Coins: %d", coin);
}

void CMario::SetState(int state)
{
	int curState = this->state;
	if (curState == state && state == MARIO_STATE_DEBUG)
	{
		this->state = MARIO_STATE_IDLE;
		return;
	}

	if (curState == MARIO_STATE_DEBUG) {
		if (state == MARIO_STATE_WALKING_LEFT) x -= 2;
		else if (state == MARIO_STATE_WALKING_RIGHT) x += 2;
		else if (state == MARIO_STATE_RUNNING_LEFT) x -= 10;
		else if (state == MARIO_STATE_RUNNING_RIGHT) x += 10;
		else if (state == MARIO_STATE_JUMP) y -= 2;
		else if (state == MARIO_STATE_SIT) y += 2;
		return;
	}


	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE || this->state == MARIO_STATE_WIN) return; 

	// run then walk mean release

	if(curState == state && curState == MARIO_STATE_SIT)
	{
		return;
	}

	if (curState == MARIO_STATE_RUNNING_LEFT || curState == MARIO_STATE_RUNNING_RIGHT || curState == MARIO_STATE_WALKING_RIGHT || curState == MARIO_STATE_WALKING_LEFT) {
		if (state == MARIO_STATE_SIT) return;
	}
	
	if (isSitting) {
		if (state == MARIO_STATE_RUNNING_LEFT || state == MARIO_STATE_RUNNING_RIGHT || state == MARIO_STATE_WALKING_RIGHT || state == MARIO_STATE_WALKING_LEFT) {
			if (isOnPlatform) {
				SetState(MARIO_STATE_SIT_RELEASE);
			}
		}
	}
	
	switch (state)
	{
	case MARIO_STATE_RUNNING_RIGHT:
		nx = 1;
		dirInput = 1;
		runInput = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		nx = -1;
		dirInput = -1;
		runInput = 1;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		nx = 1;
		dirInput = 1;
		runInput = 0;
		break;
	case MARIO_STATE_WALKING_LEFT:
		nx = -1;
		dirInput = -1;
		runInput = 0;
		break;
	case MARIO_STATE_JUMP:
		if (isOnPlatform)
		{
			isOnPlatform = false;
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
			sittingTimer->Start();
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vy = 0.0f;
			y +=MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			sittingTimer->Reset();
			isSitting = false;
			state = MARIO_STATE_IDLE;
			y -= MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_IDLE:
		dirInput = 0;
		runInput = 0;
		break;
	case MARIO_STATE_GOIN_PIPE:
		vy = vx = 0;
		break;
	case MARIO_STATE_WIN:
		vx = 0;
		vy = 0;
		holdingShell = NULL;
		dirInput = 0;
		break;
	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_SPEED_Y;
		CPlayScene* s = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		s->OnPlayerDeath();
		vx = 0;
		canHold = false;
		if(holdingShell != NULL) holdingShell->SetState(KOOPA_STATE_MARIO_DEAD); 
		holdingShell = NULL;
		if(timesup) CGameFXManager::GetInstance()->AddTimeup(0, 0, TYPE_TIMEUP);
		SetIsStop();
		break;
	
	}
	

	CGameObject::SetState(state);
}


void CMario::Acceleration(DWORD dt)
{
	//debug vx
	//DebugOutTitle(L"[INFO] Mario vx: %f\n", vx);
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
				if (glideTimer->IsRunning()) {
					if (abs(vx) <= MARIO_WALK_MAX_SPEED_X) topSpeed = MARIO_WALK_MAX_SPEED_X;
					else topSpeed = MARIO_RUN_MAX_SPEED_X ;
				}
				else if((abs(jumpVx) <= MARIO_WALK_MAX_SPEED_X && runInput == 0)) topSpeed = MARIO_WALK_MAX_SPEED_X;
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
				const float NEW_MARIO_RACCOON_MIDAIR_DECEL = 0.00006f;
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
						if(!glideTimer->IsRunning() || vx < MARIO_WALK_MAX_SPEED_X) vx += dirInput * MARIO_ACCEL_MIDAIR_X * dt;
						else {
							if(abs(vx) > MARIO_WALK_MAX_SPEED_X) vx += -dirInput * NEW_MARIO_RACCOON_MIDAIR_DECEL * dt;
						}
					}

					if (abs(vx) > topSpeed) {
						vx = topSpeed * dirInput;

					}
				}
			}
		}
	}

	if(glideTimer->IsRunning() || flyTimer->IsRunning())
	{
		// flying gliding speed is the same as walking
		//Clamp(vx, -MARIO_WALK_MAX_SPEED_X, MARIO_WALK_MAX_SPEED_X);
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
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2.f;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2.f;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else 
		{
			left = x - MARIO_BIG_BBOX_WIDTH / 2.f;
			top = y - MARIO_BIG_BBOX_HEIGHT / 2.f;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else
	{
		left = x - MARIO_SMALL_BBOX_WIDTH / 2.f;
		top = y - MARIO_SMALL_BBOX_HEIGHT / 2.f;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
}

void CMario::GetSpriteBox(float& left, float& top, float& right, float& bottom)
{
	GetBoundingBox(left, top, right, bottom);
}

void CMario::GetTailHitBox(float& l1, float& t1, float& r1, float& b1, float& l2, float& t2, float& r2, float& b2)
{
	// 1 is left hit box
	// 2 is right hit box
	if(level == MarioLevel::RACCOON)
	{	
		const float MARIO_TAIL_BBOX_WIDTH = 10.0f;
		const float MARIO_TAIL_BBOX_HEIGHT = 4.0;
		l1 = x - MARIO_BIG_BBOX_WIDTH/2 - MARIO_TAIL_BBOX_WIDTH;
		t1 = y + 2;
		r1 = l1 + MARIO_TAIL_BBOX_WIDTH;
		b1 = t1 + MARIO_TAIL_BBOX_HEIGHT;

		l2 = x + MARIO_BIG_BBOX_WIDTH / 2;
		t2 = y + 2;
		r2 = l2 + MARIO_TAIL_BBOX_WIDTH;
		b2 = t2 + MARIO_TAIL_BBOX_HEIGHT;
	}
	else
	{
		l1 = l2 = t1 = t2 = r1 = r2 = b1 = b2 = 0;
	}
}

void CMario::HoldingProcess(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float hx, hy;
	const float bigOffX = 10.f;
	const float smallOffX = 6.f;
	//holdingShell->SetPosition(x, y);
	holdingShell->GetPosition(hx, hy);

	// move the shell, also move faster when mario turn

	if (turnHoldTimer->IsRunning()) {
		holdingShell->SetFrontRender(true);
		ULONGLONG elapsed = turnHoldTimer->ElapsedTime();
		int half = elapsed < TURN_TIME / 2 ? -1 : 1;
		holdingShell->SetPosition(x + half * nx * smallOffX, y);
	}
	else {
		holdingShell->SetFrontRender(false);
		holdingShell->SetPosition(x + nx * bigOffX, y);
	}
	if (!canHold)
	{
		holdingShell->Kicked();
		KickedShell(x);
		holdingShell->ThrownInBlock(dt, coObjects);
		holdingShell = NULL;
		shellProtectTimer->Start();
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
	}else if (IsBig() && l == MarioLevel::SMALL)
	{
		y += (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
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

