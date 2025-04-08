#include "Plant.h"
#include "Game.h"
#include "Mario.h"
#include "Pipe.h"
#include "PlayScene.h"
void CPlant::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - STEM_BBOX_WIDTH / 2;
	top = y - STEM_BBOX_HEIGHT / 2;
	right = left + STEM_BBOX_WIDTH;
	bottom = top + STEM_BBOX_HEIGHT;
}

void CPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	LPGAME game = CGame::GetInstance();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if ((state == STATE_PRIRANHA_UP) && startY - y > PIRANHA_BBOX)
	{
		SetState(STATE_PRIRANHA_STOP);
	}

	if ((state == STATE_PRIRANHA_STOP) && (GetTickCount64() - up_start > PRIRANHA_STOP_TIMEOUT))
	{
		if (y < startY)
			SetState(STATE_PRIRANHA_DOWN);
		else
			SetState(STATE_PRIRANHA_UP);
	}
	if ((state == STATE_PRIRANHA_DOWN) && startY < y)
	{
		y = startY;
		SetState(STATE_PRIRANHA_STOP);
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CPlant::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);

	float yy = y - PIRANHA_BBOX / 2 + STEM_BBOX_HEIGHT / 2 + rect.right / 2;

	CGame::GetInstance()->Draw(x - cx, yy - cy, bbox, nullptr, BBOX_ALPHA, rect.right - 1, rect.bottom - 1);
}
void CPlant::Render()
{
	int aniId = ID_ANI_PRIRANHA_RED_DOWN_LEFT;
	if (state == STATE_PRIRANHA_STOP)
		aniId = ID_ANI_PRIRANHA_RED_DOWN_LEFT_STAY;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}

void CPlant::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CPlant::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;

}

void CPlant::OnCollisionWithStem(LPCOLLISIONEVENT e)
{
	CPipe* pipe = dynamic_cast<CPipe*>(e->obj);
	if (pipe != nullptr)
	{
		return;
	}
}

CPlant::CPlant(float x, float y)
{
	this->x = x;
	this->y = y;
	this->isDeleted = false;
	startY = y;
	this->up_start = GetTickCount64();
	SetState(STATE_PRIRANHA_UP);
}

void CPlant::SetState(int state)
{
	switch (state)
	{
	case STATE_PRIRANHA_UP:
		vy = -PIRANHA_SPEED;
		up_start = 0;
		break;
	case STATE_PRIRANHA_STOP:
		if (up_start == 0)
			up_start = GetTickCount64();
		vy = 0;
		break;
	case STATE_PRIRANHA_DOWN:
		vy = PIRANHA_SPEED;
		up_start = 0;
		break;
	}
	CGameObject::SetState(state);
}
