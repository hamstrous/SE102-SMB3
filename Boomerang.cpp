#include "Boomerang.h"
#include "BoomerangBro.h"
#include "Collision.h"

void CBoomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	CCollision::GetInstance()->Process(this, dt, coObjects);

	 if (state == BOOMERANG_STATE_MOVING_UP)
	{
		 if(y <= startY - BOOMERANG_VERTICAL_LENGTH)
		 {
			 SetState(BOOMERANG_STATE_FLYING_UP);
		 }
	}
	else if (state == BOOMERANG_STATE_FLYING_UP)
	{
		 if(abs(startX - x) >= BOOMERANG_HORIZONTAL_LENGTH)
		 {
			 SetState(BOOMERANG_STATE_MOVING_DOWN);
		 }
	}else if(state == BOOMERANG_STATE_MOVING_DOWN)
	{
		 if(y >= startY)
		 {
			 SetState(BOOMERANG_STATE_FLYING_DOWN);
		 }
	}
}

void CBoomerang::Render()
{
	if (GetIsPause()) return;
	int aniId = ID_ANI_BOOMERANG_FLYING_RIGHT;

	if(state == BOOMERANG_STATE_HOLDING)
	{
		aniId = nx > 0 ? ID_ANI_BOOMERANG_HOLDING_RIGHT : ID_ANI_BOOMERANG_HOLDING_LEFT;
	}
	else
	{
		aniId = nx > 0 ? ID_ANI_BOOMERANG_FLYING_RIGHT : ID_ANI_BOOMERANG_FLYING_LEFT;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void CBoomerang::SetState(int state)
{
	switch (state)
	{
	case BOOMERANG_STATE_FLYING_UP:
		ay = 0;
		vy = 0;
		break;
	case BOOMERANG_STATE_FLYING_DOWN:
		ay = 0;
		vy = 0;
		break;
	case BOOMERANG_STATE_MOVING_UP:
		ay = -BOOMERANG_VERTICAL_ACCE;
		ax = nx * BOOMERANG_HORIZONTAL_ACCE;
		break;
	case BOOMERANG_STATE_MOVING_DOWN:
		ay = 0;
		vy = BOOMERANG_VERTICAL_SPEED;
		ax = -nx * BOOMERANG_HORIZONTAL_ACCE;
		break;
	case BOOMERANG_STATE_HOLDING:
		vx = 0;
		vy = 0;
		ax = ay = 0;
		break;
	default:
		break;
	}
	CGameObject::SetState(state);
}

void CBoomerang::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - BOOMERANG_BBOX_WIDTH / 2;
	top = y - BOOMERANG_BBOX_HEIGHT / 2;
	right = left + BOOMERANG_BBOX_WIDTH;
	bottom = top + BOOMERANG_BBOX_HEIGHT;
	if (state == BOOMERANG_STATE_HOLDING)
	{
		left = top = right = bottom = 0;
	}
}

void CBoomerang::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
	/*if (state == BOOMERANG_STATE_TURNING_UP)
	{
		if (upTimer->IsTimeUp())
		{
			SetState(BOOMERANG_STATE_TURNING_DOWN);
			upTimer->Reset();
		}
	}
	else if (state == BOOMERANG_STATE_TURNING_DOWN)
	{
		if (upTimer->IsTimeUp())
		{
			SetState(BOOMERANG_STATE_FLYING);
			upTimer->Reset();
		}
	}*/
}

void CBoomerang::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if(dynamic_cast<CBoomerangBro*>(e->obj))
	{
		OnCollisionWithBoomerangBro(e);
	}
}
