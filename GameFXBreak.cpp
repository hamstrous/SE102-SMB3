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
	vy += ay * dt;
	vy = min(vy, 0.2f);
	x += vx * dt;
	y += vy * dt;
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
