#include "BreakableBrick.h"
#include "ScoreManager.h"
#include "GameData.h"
#include "GameFXBreak.h"
#include "GameFXManager.h"
#include "Mario.h"
#include "Coin.h"
#include "PlayScene.h"
void CBreakableBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if(CGame::GetInstance()->GetChangeBricktoCoin()) animations->Get(ID_ANI_COIN_TYPE2)->Render(x, y);
	else if (unbox) animations->Get(QUESTION_BLOCK_STATE_UNBOX)->Render(x, y);
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
	CCollision::GetInstance()->ProcessCollision(this, dt, coObjects);
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
	if (!CGame::GetInstance()->GetChangeBricktoCoin() && type != TYPE_ADDSCORE && !CGame::GetInstance()->GetChangeBricktoCoin())
	{
		CScoreManager::GetInstance()->AddScore(x, y, 10);
		isDeleted = true;
		CGameFXManager::GetInstance()->AddBreak(x, y);
	}
	if (type == TYPE_ADDSCORE)
	{
		AddScoreBrick();
	}
}

void CBreakableBrick::BottomHit()
{	
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	if (mario->GetLevel() != MarioLevel::SMALL && type != TYPE_ADDSCORE && !CGame::GetInstance()->GetChangeBricktoCoin())
	{
		CScoreManager::GetInstance()->AddScore(x, y, 10);
		isDeleted = true;
		CGameFXManager::GetInstance()->AddBreak(x, y);
	}
	else {
		bouncing = true;
		time_start = GetTickCount64();
	}
	if (type == TYPE_ADDSCORE)
	{
		AddScoreBrick();
	}
}

void CBreakableBrick::AddScoreBrick()
{
	LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	CCoin* coin = new CCoin(x, y, TYPE_1);
	coin->SetState(COIN_STATE_MOVEUP);
	scene->AddObject(coin);
	CGameData::GetInstance()->AddCoin(1);
	bouncing = true;
	time_start = GetTickCount64();
}
