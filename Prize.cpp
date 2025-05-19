#include "Prize.h"
#include "Collision.h"
#include "Mario.h"
#include "Game.h"
#include "PlayScene.h"

void CPrize::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - PRIZE_BBOX_WIDTH/2;
	t = y - PRIZE_BBOX_HEIGHT/2;
	r = l + PRIZE_BBOX_WIDTH;
	b = t + PRIZE_BBOX_HEIGHT;
}

void CPrize::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if(dynamic_cast<CMario*>(e->obj))
	{
		SetState(PRIZE_STATE_UP);
	}
}

void CPrize::Render() { 
	if(state == PRIZE_STATE_FLICKER) CSprites::GetInstance()->Get(spriteId[type])->Draw(x, y);
	else if (state == PRIZE_STATE_UP) CAnimations::GetInstance()->Get(animationId[type])->Render(x, y);
	
}

void CPrize::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	CCollision::GetInstance()->Process(this, dt, coObjects);
	if(state == PRIZE_STATE_FLICKER)
	{
		if (changeTimer->IsDone())
		{
			changeTimer->Start();
			type++;
			type %= 3;
		}
	}
	else if (state == PRIZE_STATE_UP)
	{
		y -= dt * PRIZE_UP_SPEED;
	}


}

void CPrize::SetState(int state) { 
	switch (state) {
	case PRIZE_STATE_FLICKER:
		changeTimer->Start();
		break;
	case PRIZE_STATE_UP:
		changeTimer->Reset();
		killOffCam = true;
		CPlayScene* scene = (CPlayScene*)( CGame::GetInstance()->GetCurrentScene());
		scene->OnPlayerWin(type);
		break;
	}
	CGameObject::SetState(state);
}

