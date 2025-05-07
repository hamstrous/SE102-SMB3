#include "GameFX.h"

void CGameFX::Render()
{
	if (GetIsPause()) {
		return;
	}
	switch (type)
	{
	case TYPE_TAILHIT:
		int aniId = ANI_ID_TAILHIT;
		if (GetTickCount64() - start <= TIME_TAILHIT)
		{
			if (!GetIsStop()) CAnimations::GetInstance()->Get(aniId)->Render(x, y);
			else CAnimations::GetInstance()->Get(aniId)->Render(x, y, 1);
		}
		break;
	}
}

void CGameFX::Update(DWORD dt)
{	
	switch (type)
	{
	case TYPE_TAILHIT:
		if (GetTickCount64() - start >= TIME_TAILHIT)
		{
			isDeleted = true;
			return;
		}
		break;
	}
	
}

void CGameFX::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}

void CGameFX::SetState(int state)
{
}
