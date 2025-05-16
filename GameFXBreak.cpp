#include "GameFXBreak.h"

void CGameFXBreak::Render()
{
	if (GetTickCount64() - start <= TIME_BREAK)
	{
		if (!GetIsStop()) CAnimations::GetInstance()->Get(ANI_ID_BREAKABLEBRICK)->Render(x, y);
		else CAnimations::GetInstance()->Get(ANI_ID_BREAKABLEBRICK)->Render(x, y, 1);
	}
}

void CGameFXBreak::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{	
	x += vx * dt;
	y += vy * dt;
	if (type == TYPE_LEFT_BOT_BREAK || type == TYPE_RIGHT_BOT_BREAK)
	{
		SetState(STATE_LOW_JUMP);
	}
	else if (type == TYPE_LEFT_TOP_BREAK || type == TYPE_RIGHT_TOP_BREAK)
	{	
		SetState(STATE_HIGH_JUMP);
	}

	if (state == STATE_LOW_JUMP && GetTickCount64() - start >= 120)
	{
		vx = vx > 0 ? SPEED_X3 : -SPEED_X3;
		vy = DOWN_LOW_JUMP;
	}
	else if (state == STATE_HIGH_JUMP && GetTickCount64() - start >= 320)
	{
		vx = vx > 0 ? SPEED_X3 : -SPEED_X3;
		vy = DOWN_HIG_JUMP;
	}
	CGameObject::Update(dt, coObjects);
}

void CGameFXBreak::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}

void CGameFXBreak::SetState(int state)
{
	switch (state)
	{
	case STATE_LOW_JUMP:
		vy = -LOW_JUMP;
		break;
	case STATE_HIGH_JUMP:
		vy = -HIGH_JUMP;
	default:
		break;
	}
	CGameObject::SetState(state);
}

//void CGameFXBreak::OnNoCollision(DWORD dt)
//{
//	x += vx * dt;
//	y += vy * dt;
//}
//
//void CGameFXBreak::OnCollisionWith(LPCOLLISIONEVENT e, DWORD dt)
//{
//	x += vx * dt;
//	y += vy * dt;
//}


