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
	if (GetIsPause() && (unbox|| bouncing)) return;
	CAnimations* animations = CAnimations::GetInstance();
	if(CGame::GetInstance()->GetChangeBricktoCoin()) animations->Get(ID_ANI_COIN_TYPE2)->Render(x, y);
	else if (unbox) animations->Get(ID_ANI_BLOCK_UNBOX_BOUNCING)->Render(x, y);
	else if (unbox2)  animations->Get(ID_ANI_BLOCK_UNBOX)->Render(x, y);
	else if (bouncing) {
		//animations->Get(ID_ANI_BOUNCING)->ResetBouncing();
		animations->Get(ID_ANI_BOUNCING)->Render(x, y);
	}
	else animations->Get(ID_ANI_BREAKABLEBRICK)->Render(x, y);
	//RenderBoundingBox();
}

void CBreakableBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{	
	y += vy * dt;


	if (time_start != -1 && GetTickCount64() - time_start >= 180)
	{
		time_start = -1;
		bouncing = false;
		//unbox = false;
		if (type != 0 && unbox == true) {
			unbox2 = true;
			unbox = false;
		}
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
	if(invincibleTimer->IsRunning()) return;
	invincibleTimer->Start();
	if (!CGame::GetInstance()->GetChangeBricktoCoin() && type != TYPE_ADDSCORE && !CGame::GetInstance()->GetChangeBricktoCoin())
	{
		CScoreManager::GetInstance()->AddScore(x, y, 10);
		isDeleted = true;
		CGameFXManager::GetInstance()->AddBreak(x, y);
	}

	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);

	if (type == TYPE_ADDSCORE && !unbox2)
	{
		AddScoreBrick();
		if (cx + 30 >= x) unbox = true;
		time_start = GetTickCount64();
	}
}

void CBreakableBrick::BottomHit()
{	
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();

	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);

	if (mario->GetLevel() != MarioLevel::SMALL && type != TYPE_ADDSCORE && !CGame::GetInstance()->GetChangeBricktoCoin())
	{
		bouncing = true;
		CScoreManager::GetInstance()->AddScore(x, y, 10);
		isDeleted = true;
		CGameFXManager::GetInstance()->AddBreak(x, y);
	}
	else {
		bouncing = true;
		time_start = GetTickCount64();
	}
	if (type == TYPE_ADDSCORE && !unbox2){
		AddScoreBrick();
		if (cx + 30 >= x) unbox = true;
		time_start = GetTickCount64();
	}
}

void CBreakableBrick::AddScoreBrick()
{
	LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	CCoin* coin = new CCoin(x, y, TYPE_1);
	coin->SetState(COIN_STATE_MOVEUP);
	scene->AddObject(coin);
	CGameData::GetInstance()->AddCoin(1);
	if (!bouncing) {
		bouncing = true;
		time_start = GetTickCount64();
	}
	time_start = GetTickCount64();
}
