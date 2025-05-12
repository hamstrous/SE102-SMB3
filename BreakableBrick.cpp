#include "BreakableBrick.h"
#include "ScoreManager.h"
#include "GameData.h"
void CBreakableBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if(CGame::GetInstance()->GetChangeBricktoCoin()) animations->Get(ID_ANI_COIN_TYPE2)->Render(x, y);
	else if(bouncing) animations->Get(ID_ANI_BOUNCING)->Render(x, y);
	else animations->Get(ID_ANI_BREAKABLEBRICK)->Render(x, y);
	//RenderBoundingBox();
}

void CBreakableBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{	
	y += vy * dt;
	if (GetTickCount64() - time_start >= 230)
	{
		time_start = -1;
		bouncing = false;
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CBreakableBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x - BRICK_BBOX_WIDTH/2;
	t = y - BRICK_BBOX_HEIGHT/2;
	r = l + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;
}

void CBreakableBrick::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CMario*>(e->obj) && CGame::GetInstance()->GetChangeBricktoCoin())
	{
		isDeleted = true;
		CGameData::GetInstance()->AddCoin(1);
		CGameData::GetInstance()->AddScore(50);
	}
}

void CBreakableBrick::SetState(int state)
{
	switch (state)
	{
	case STATE_MOVE_UP:
		vy = -0.02f;
		time_start = GetTickCount64();
		break;
	case STATE_MOVE_DOWN:
		vy = 0.02f;
		break;
	}

}

void CBreakableBrick::SideHit()
{
	if (!CGame::GetInstance()->GetChangeBricktoCoin())
	{
		isDeleted = true;
		CScoreManager::GetInstance()->AddScore(x, y, 10);
	}
}

void CBreakableBrick::BottomHit()
{	
	bouncing = true;
	time_start = GetTickCount64();
	/*SetState(STATE_MOVE_UP);
	DebugOut(L"State_move_up");*/
}
