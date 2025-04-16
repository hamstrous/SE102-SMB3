#include "Leaf.h"

void CLeaf::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - LEAF_BBOX_WIDTH / 2;
	top = y - LEAF_BBOX_HEIGHT / 2;
	right = left + LEAF_BBOX_WIDTH;
	bottom = top + LEAF_BBOX_HEIGHT;

}

void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == LEAF_STATE_UP && startY - y >= LEAF_UP_DISTANCE)
	{
		SetState(LEAF_STATE_STOP);
	}
	if (state == LEAF_STATE_STOP && (GetTickCount64() - time_stop >= LEAF_TIME_STOP))
	{
		SetState(LEAF_STATE_FALL);
		
	}
	/*if (state == LEAF_STATE_FLY_UP && GetTickCount64() - fallReverse_start > LEAF_BLOW_UP_TIME)
	{
		SetState(LEAF_STATE_FALL);
	}*/
	vy += ay * dt;
	vx += ax * dt;
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CLeaf::Render()
{
	int aniId = ID_ANI_LEAF_RIGHT;
	if (vx < 0) aniId = ID_ANI_LEAF_LEFT;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void CLeaf::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
	//DebugOut(L"Leaf: %f %f \n", x, y);
}

void CLeaf::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
}

void CLeaf::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case LEAF_STATE_UP:
		vy = -LEAF_SPEED_UP;
		vx = 0;
		ax = 0;
		ay = 0;
		break;
	case LEAF_STATE_FALL:
		vx = LEAF_SPEED/4;
		ax = 0;
		ay = LEAF_GRAVITY;
		fallReverse_start = GetTickCount64();
		break;
	case LEAF_STATE_FLY_UP:
		vx = -LEAF_SPEED;
		ax = 0;
		ay = -LEAF_GRAVITY;
		break;
	case LEAF_STATE_STOP:
		vx = 0;
		vy = 0;
		time_stop = GetTickCount64();
		break;

	default:
		break;
	}
}
