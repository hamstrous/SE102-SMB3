#include "Fireball.h"
#include "debug.h"
#include "Mario.h"

void CFireball::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - FIREBALL_BBOX_WIDTH / 2;
	top = y - FIREBALL_BBOX_HEIGHT / 2;
	right = left + FIREBALL_BBOX_WIDTH;
	bottom = top + FIREBALL_BBOX_HEIGHT;
}

void CFireball::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (GetTickCount64() - time_start > FIREBALL_TIME_DELETE)
		Delete();
	CGameObject::Update(dt, coObjects);
	//CCollision::GetInstance()->ProcessOnlyMario(this, dt, coObjects);
	OnNoCollision(dt);
}

void CFireball::Render()
{	
	if (GetIsPause()) {
		time_start = GetTickCount64();
		return;
	}
	int aniId = ID_ANI_FIREBALL; 
	if (!GetIsStop()) CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	else CAnimations::GetInstance()->Get(aniId)->Render(x, y, 1);
	RenderBoundingBox();
}

void CFireball::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CFireball::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CMario*>(e->obj))
	{
		DebugOut(L"[INFO] Fireball hit Mario\n");
	}
}




void CFireball::SetState(int state)
{
	switch (state)
	{
	case FIREBALL_STATE_MARIO:
		break;
	case FIREBALL_STATE_PIRANHA:
		break;
	}
}
