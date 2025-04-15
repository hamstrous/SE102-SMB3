#include "Score.h"

void CScore::Render()
{
	int aniId = ID_ANI_SCORE_100;
	switch (score)
	{
	case 100:
		aniId = ID_ANI_SCORE_100;
		break;
	case 200:
		aniId = ID_ANI_SCORE_200;
		break;
	case 400:
		aniId = ID_ANI_SCORE_400;
		break;
	case 800:
		aniId = ID_ANI_SCORE_800;
		break;
	case 1000:
		aniId = ID_ANI_SCORE_1000;
		break;
	case 2000:
		aniId = ID_ANI_SCORE_2000;
		break;
	case 4000:
		aniId = ID_ANI_SCORE_4000;
		break;
	case 8000:
		aniId = ID_ANI_SCORE_8000;
		break;
	default:
		break;
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CScore::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	y += vy * dt;
	if (y < startY - SCORE_DISTANCE_UP && state == SCORE_STATE_UP)
	{
		SetState(SCORE_STATE_DONE);
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CScore::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case SCORE_STATE_INIT:
		vy = 0;
		break;
	case SCORE_STATE_UP:
		vy = -SCORE_SPEED;
		break;
	case SCORE_STATE_DONE:
		Delete();
		break;
	}
}
