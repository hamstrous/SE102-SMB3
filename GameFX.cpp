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
		CAnimations::GetInstance()->Get(ANI_ID_TIMEUP)->Render(x, y);
	}
	default:
		break;
	}
}

void CGameFX::Update(DWORD dt)
{		
	CGame* game = CGame::GetInstance();
	float hy = game->GetBackBufferHeight() - HUD_SIZE_Y;
	switch (type)
	{
		case TYPE_TAILHIT:
		{
			if (GetTickCount64() - start >= TIME_TAILHIT) isDeleted = true;
			break;
		}
		case TYPE_TIMEUP:
		{
			if (GetTickCount64() - start >= 1000)
				isDeleted = true;
			break;
		}
	}
	/*y += vy * dt;
	x += vx * dt;*/
}

void CGameFX::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}

void CGameFX::SetState(int state)
{
	switch (state)
	{
	case STATE_TIME_MOVEUP:
		isOver = true;
		vy = -0.5f;
		break;
	case STATE_TIME_STOP:
		vy = 0.0f;
		break;
	case STATE_OTHER:
		break;
	default:
		break;
	}
	CGameObject::SetState(state);
}
