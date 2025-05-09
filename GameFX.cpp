#include "GameFX.h"
#include "Game.h"
void CGameFX::Render()
{	
	CGame* game = CGame::GetInstance();
	float hx = game->GetBackBufferWidth() / 2;
	float hy = game->GetBackBufferHeight() - HUD_SIZE_Y;
	if (isOn) {
		CAnimations::GetInstance()->Get(ANI_ID_GAMEPAUSE)->Render(hx, hy + OFFSET_PAUSE_HEIGHT);
		return;
	}
	if (GetIsPause()) {
		return;
	}
	switch (type)
	{
	case TYPE_TAILHIT:
	{
		if (GetTickCount64() - start <= TIME_TAILHIT)
		{
			if (!GetIsStop()) CAnimations::GetInstance()->Get(ANI_ID_TAILHIT)->Render(x, y);
			else CAnimations::GetInstance()->Get(ANI_ID_TAILHIT)->Render(x, y, 1);
		}
		break;
	}
	case TYPE_TIMEUP:
	{	
		if (GetTickCount64() - start <= 5000)
		{	
			if (!GetIsStop()) CAnimations::GetInstance()->Get(ANI_ID_TIMEUP)->Render(x, y );
			else CAnimations::GetInstance()->Get(ANI_ID_TIMEUP)->Render(x, y, 1);
		}
		break;
	}
	default:
		break;
	}
}

void CGameFX::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{		
	//DebugOut(L"Type: %d, Time elapsed: %llu\n", type, GetTickCount64() - start);
	
	y += vy * dt;
	//DebugOut(L"x: %f, y: %f\n", x, y);
	CGame* game = CGame::GetInstance();
	float hy = game->GetBackBufferHeight() + 100;
	switch (type)
	{
		case TYPE_TAILHIT:
		{
			if (GetTickCount64() - start >= TIME_TAILHIT) isDeleted = true;
			break;
		}
		case TYPE_TIMEUP:
		{	
			
			if (y >= hy)
			{
				vy = -0.7f;
			}
			else vy = 0;
		}
	}
	CGameObject::Update(dt, coObjects);
}

void CGameFX::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}

void CGameFX::SetState(int state)
{
	switch (state)
	{
	}
	CGameObject::SetState(state);
}
