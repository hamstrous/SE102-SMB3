#include "GameFX.h"
#include "Game.h"
void CGameFX::Render()
{
	if (isOn) {
		CGame* game = CGame::GetInstance();
		float hx = game->GetBackBufferWidth() / 2;
		float hy = game->GetBackBufferHeight() - HUD_SIZE_Y;
		CAnimations::GetInstance()->Get(ANI_ID_GAMEPAUSE)->Render(hx + OFFSET_PAUSE_WIDTH, hy + OFFSET_PAUSE_HEIGHT);
		return;
	}
	if (GetIsPause()) {
		return;
	}
	CGame* game = CGame::GetInstance();
	float hx = game->GetBackBufferWidth() / 2;
	float hy = game->GetBackBufferHeight() - HUD_SIZE_Y;
	switch (type)
	{
	case TYPE_TAILHIT:
	{
		int aniId = ANI_ID_TAILHIT;
		if (GetTickCount64() - start <= TIME_TAILHIT)
		{
			if (!GetIsStop()) CAnimations::GetInstance()->Get(aniId)->Render(x, y);
			else CAnimations::GetInstance()->Get(aniId)->Render(x, y, 1);
		}
		break;
	}
	default:
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
