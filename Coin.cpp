#include "Coin.h"

void CCoin::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (!isDeleted)
		animations->Get(ID_ANI_COIN)->Render(x, y);
	//RenderBoundingBox();
}

void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == COIN_STATE_MOVEUP && startY - y >= DISTANCE_COIN_MOVEUP)
	{
		SetState(COIN_STATE_MOVEDOWN);
	}
	if ( vy > 0 && state == COIN_STATE_MOVEDOWN && startY - y <= DISTANCE_COIN_MOVEDOWN)
	{
		SetState(COIN_STATE_GONE);
	}
	y += vy * dt;
}


void CCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - COIN_BBOX_WIDTH / 2;
	t = y - COIN_BBOX_HEIGHT / 2;
	r = l + COIN_BBOX_WIDTH;
	b = t + COIN_BBOX_HEIGHT;
}

void CCoin::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case COIN_STATE_NORMAL:
		vy = 0;
		break;
	case COIN_STATE_GONE:
		Delete();
		break;
	case COIN_STATE_MOVEUP:
		vy = -SPEED_COIN;
		break;
	case COIN_STATE_MOVEDOWN:
		vy = SPEED_COIN;
		break;
	}

}
