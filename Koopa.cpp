#include "Koopa.h"

CKoopa::CKoopa(float x, float y)
{
	this->x = x;
	this->y = y;
	vx = KOOPA_WALKING_SPEED;
	ay = KOOPA_GRAVITY;
	SetState(KOOPA_STATE_WALKING);
}

void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - KOOPA_BBOX_WIDTH / 2;
	top = y - KOOPA_BBOX_HEIGHT / 2;
	right = left + KOOPA_BBOX_WIDTH;
	bottom = top + KOOPA_BBOX_HEIGHT;
}

void CKoopa::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CKoopa*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	/*if ((state == GOOMBA_STATE_DIE) && (GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}*/

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CKoopa::Render()
{
	int aniId = 0;
	if(vx > 0) 
		aniId = ID_ANI_KOOPA_WALKING_RIGHT;
	else if(vx < 0) 
		aniId = ID_ANI_KOOPA_WALKING_LEFT;
	else
		aniId = ID_ANI_KOOPA_WALKING_RIGHT;

	if (state == KOOPA_STATE_DIE)
	{
		aniId = ID_ANI_KOOPA_DIE;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void CKoopa::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPA_STATE_DIE:
		vx = 0;
		vy = 0;
		break;
	case KOOPA_STATE_WALKING:
		vx = KOOPA_WALKING_SPEED;
		break;
	}
}
