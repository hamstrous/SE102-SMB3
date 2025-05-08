#include "Coin.h"
#include "ScoreManager.h"
void CCoin::Render()
{	
	if (GetIsPause()) return;
	CAnimations* animations = CAnimations::GetInstance();
	if (!isDeleted)
		animations->Get(ID_ANI_COIN)->Render(x, y);
	if (!GetIsStop()) CAnimations::GetInstance()->Get(ID_ANI_COIN)->Render(x, y);
	else CAnimations::GetInstance()->Get(ID_ANI_COIN)->Render(x, y, 1);
	//RenderBoundingBox();
}

void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == COIN_STATE_MOVEUP && startY - y >= DISTANCE_COIN_MOVEUP)
	{
		SetState(COIN_STATE_HOLDUP);
	}
	if (state == COIN_STATE_HOLDUP && startY - y >= DISTANCE_COIN_MOVEUP - DISTANCE_COIN_HOLDUP)
	{
		SetState(COIN_STATE_MOVEDOWN);
	}
	if ( vy > 0 && state == COIN_STATE_MOVEDOWN && startY - y <= DISTANCE_COIN_MOVEDOWN)
	{
		SetState(COIN_STATE_GONE);
	}
	y += vy * dt;
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);

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
	LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	CGameObject::SetState(state);
	switch (state)
	{
	case COIN_STATE_NORMAL:
		vy = 0;
		break;
	case COIN_STATE_GONE:
		{
		CScoreManager::GetInstance()->AddScore(x, y, 100);
		Delete();
		break;
		}
		
	case COIN_STATE_MOVEUP:
		vy = -SPEED_COIN_FAST;
		break;
	case COIN_STATE_MOVEDOWN:
		vy = SPEED_COIN_FAST;
		break;
	case COIN_STATE_HOLDUP:
		vy = SPEED_COIN_SLOW;
		break;
	}

}
