#include "BoomerangBro.h"
#include "Collision.h"
#include "PlayScene.h"
#include "Game.h"

void CBoomerangBro::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vx += 0;
	vy += BOOMERANG_BRO_GRAVITY * dt;
	float playerX, playerY;
	GetPlayerPosition(playerX, playerY);
	if (!IsDead()) {
		if (playerX < x)
		{
			nx = -1;
		}
		else
		{
			nx = 1;
		}
	}
	CCollision::GetInstance()->ProcessCollision(this, dt, coObjects);

	if (IsDead()) return;

	if (jumpTimer->IsDone()) {
		vy = -BOOMERANG_BRO_JUMP_SPEED;
		jumpTimer->SetTimeSpan(random.GetInt(8000,12000));
		jumpTimer->Start();
	}

	float minX, maxX, midX;
	if(toLeft) 	{
		minX = startX - BOOMERANG_WALK_LENGTH;
		maxX = startX;
	}
	else
	{
		minX = startX;
		maxX = startX + BOOMERANG_WALK_LENGTH;
	}
	midX = (minX + maxX) / 2;

	if (x < minX) {
		x = minX; 
		vx = 0;
		waitTimer->Start();
	}
	if (x > maxX) {
		x = maxX;
		vx = 0;
		waitTimer->Start();
	}

	if((vx > 0 && x >= midX && cycleChecker == toLeft) || (vx < 0 && x <= midX && cycleChecker != toLeft))
	{
		canThrow = true;
		cycleCounter++;
		cycleChecker = !cycleChecker;

	}
	if(cycleCounter >= 4) {
		cycleCounter = 0;
	}

	if((cycleCounter == 1 || cycleCounter == 2) && boomerang == NULL && canThrow) {
		throwTimer->Start();
		SetState(BOOMERANG_BRO_STATE_THROWING);
		//DebugOut(L"[INFO] Throwing\n");
		canThrow = false;
	}

	if(boomerang != NULL)
	{
		if (throwTimer->IsDone()) {
			SetState(BOOMERANG_BRO_STATE_WALKING);
			throwTimer->Reset();
			boomerang->SetStartFlying(nx);
			boomerang = NULL;
		}
		else 
			boomerang->SetHold(x + nx * -7, y - 7, nx);
		
	}
	

	if (waitTimer->IsDone()) {
		if(x>=maxX) vx = -BOOMERANG_BRO_WALKING_SPEED;
		else if (x <= minX) vx = BOOMERANG_BRO_WALKING_SPEED;
		waitTimer->Reset();
	}
}

void CBoomerangBro::Render()
{
	if (GetIsPause()) return;
	int aniId = ID_ANI_BOOMERANG_BRO_WALKING_LEFT;
	if (state == BOOMERANG_BRO_STATE_DIE) {
		aniId = nx > 0 ? ID_ANI_BOOMERANG_BRO_DIE_RIGHT : ID_ANI_BOOMERANG_BRO_DIE_LEFT;
	}
	else if (state == BOOMERANG_BRO_STATE_WALKING) {
		aniId = (nx < 0 ? ID_ANI_BOOMERANG_BRO_WALKING_LEFT : ID_ANI_BOOMERANG_BRO_WALKING_RIGHT);
	}
	else if (state == BOOMERANG_BRO_STATE_THROWING) {
		aniId = (nx < 0 ? ID_ANI_BOOMERANG_BRO_THROWING_LEFT : ID_ANI_BOOMERANG_BRO_THROWING_RIGHT);
	}
	int mode = 0;
	if (waitTimer->IsRunning()) mode = 1;
	if (!GetIsStop() && !GetIsDead()) mode = 1;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y, mode);
	RenderBoundingBox();
}

void CBoomerangBro::SetState(int state)
{
	if(this->state == BOOMERANG_BRO_STATE_THROWING && state == BOOMERANG_BRO_STATE_WALKING) {
		CGameObject::SetState(state);
		return;
	}
	switch (state) {
		case BOOMERANG_BRO_STATE_WALKING:
			if(toLeft == -1) toLeft = random.GetInt(0, 1);
			vx = (toLeft ? -1 : 1) * BOOMERANG_BRO_WALKING_SPEED;
			vy = 0;
			break;
		case BOOMERANG_BRO_STATE_THROWING:
			if (boomerang == NULL) {
				boomerang = new CBoomerang(x,y);
				CGame* game = CGame::GetInstance();
				CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
				scene->AddObject(boomerang);
			}
			break;
		case BOOMERANG_BRO_STATE_DIE:
			if(boomerang != NULL) {
				boomerang->Delete();
				boomerang = NULL;
			}
			vx = 0;
			killOffCam = true;
			break;
	}
	CGameObject::SetState(state);
}

void CBoomerangBro::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (IsDead()) {
		left = top = right = bottom = 0;
		return;
	
	}
	left = x - BOOMERANG_BRO_BBOX_WIDTH / 2;
	top = y - BOOMERANG_BRO_BBOX_HEIGHT / 2;
	right = left + BOOMERANG_BRO_BBOX_WIDTH;
	bottom = top + BOOMERANG_BRO_BBOX_HEIGHT;
}

void CBoomerangBro::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CBoomerangBro::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->obj->IsBlocking()) {
		if (e->ny != 0)
		{
			vy = 0;
		}
		else if (e->nx != 0)
		{
			vx = -vx;
		}
	}
}

