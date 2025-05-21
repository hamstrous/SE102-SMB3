#include "Boomerang.h"
#include "BoomerangBro.h"
#include "Collision.h"
#include "debug.h"

void CBoomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vy = min(vy, BOOMERANG_MAX_VERTICAL_SPEED);
	CCollision::GetInstance()->ProcessNoBlock(this, dt, coObjects);
	DebugOutTitle(L"Boomerang: %f, %f\n", x, y);
	if (state == BOOMERANG_STATE_OUTWARD)
	{
		BOOMERANG_VERTICAL_LENGTH = max(BOOMERANG_VERTICAL_LENGTH, abs(y - startY));
		if (abs(x - startX) < BOOMERANG_HORIZONTAL_LENGTH) vy = min(vy, 0);
		if (vy > 0 && startY - y <= BOOMERANG_VERTICAL_LENGTH / 2)
		{
			SetState(BOOMERANG_STATE_RETURN);
		}
	}
	else if (state == BOOMERANG_STATE_RETURN) {
		if (y >= startY) vy = 0, y = startY;
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
	case BOOMERANG_STATE_OUTWARD:
		vy = -BOOMERANG_INITAL_UPWARD_SPEED;
		ay = BOOMERANG_GRAVITY;
		vx = BOOMERANG_HORIZONTAL_SPEED * nx;
		startX = x;
		startY = y + 12;
		break;
	case BOOMERANG_STATE_RETURN:
		vx = -vx;
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
