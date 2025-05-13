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

	CGameObject::Update(dt, coObjects);
}

void CGameFXBreak::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}

void CGameFXBreak::SetState(int state)
{
}
