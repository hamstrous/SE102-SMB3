#include "Smoke.h"
#include "debug.h"

void CSmoke::Render()
{	
	if (GetIsPause()) {
		start = GetTickCount64();
		return;
	}
	int aniId = ID_ANI_SMOKE;
	if (GetTickCount64() - start <= SMOKE_TIME)
	{
		if (!GetIsStop()) CAnimations::GetInstance()->Get(aniId)->Render(x, y);
		else CAnimations::GetInstance()->Get(aniId)->Render(x, y, 1);
	}
	//RenderBoundingBox();
}

void CSmoke::Update(DWORD dt)
{
	if (GetTickCount64() - start >= SMOKE_TIME)
	{
		isDeleted = true;
		DebugOut(L"[INFO] Smoke deleted\n");
		return;
	}
}

void CSmoke::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}

void CSmoke::SetState(int state)
{
	CGameObject::SetState(state);
}
