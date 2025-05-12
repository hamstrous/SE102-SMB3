#include "BreakableBrick.h"
#include "ScoreManager.h"
#include "GameData.h"
void CBreakableBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if(CGame::GetInstance()->GetChangeBricktoCoin()) animations->Get(ID_ANI_COIN_TYPE2)->Render(x, y);
	else animations->Get(ID_ANI_BREAKABLEBRICK)->Render(x, y);
	//RenderBoundingBox();
}

void CBreakableBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
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
	if (dynamic_cast<CMario*>(e->obj))
	{
		DebugOut(L"[INFO] Mario collided with BreakableBrick\n");
	}

	if (CGame::GetInstance()->GetChangeBricktoCoin())
	{
		DebugOut(L"[INFO] ChangeBricktoCoin is true\n");
	}

	if (dynamic_cast<CMario*>(e->obj) && CGame::GetInstance()->GetChangeBricktoCoin())
	{
		DebugOut(L"[INFO] BreakableBrick is deleted\n");
		isDeleted = true;
		CGameData::GetInstance()->AddCoin(1);
		CGameData::GetInstance()->AddScore(50);
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

}
