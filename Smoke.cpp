#include "Smoke.h"
#include "debug.h"

void CSmoke::Render()
{
	int aniId = ID_ANI_SMOKE;
	if (GetTickCount64() - start <= SMOKE_TIME)
	{
		CAnimations::GetInstance()->Get(aniId)->Render(x, y);
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
