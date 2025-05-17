#include "MovingPlatform.h"
#include "Mario.h"

void CMovingPlatform::Render()
{
	float xx = x - 16;
	float xxx = x + 16;
	CSprites* s = CSprites::GetInstance();
	s->Get(ID_ANI_BODY)->Draw(x, y);
	s->Get(ID_ANI_HEAD)->Draw(xx, y);
	s->Get(ID_ANI_LEG)->Draw(xxx, y);
	//RenderBoundingBox();
}

void CMovingPlatform::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - BBOX_WIDTH / 2;
	t = y - BBOX_HEIGHT / 2;
	r = l + BBOX_WIDTH;
	b = t + BBOX_HEIGHT;
}

void CMovingPlatform::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	x += vx * dt;
	if (isActive) {
		y += SPEED_Y * dt;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMovingPlatform::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CMario*>(e->obj))
	{
		CMario* mario = dynamic_cast<CMario*>(e->obj);
		mario->SetIsOnPlatform();
		float mx, my;
		mario->GetPosition(mx, my);
		// Move Mario down with platform
		mario->SetPosition(mx, my + SPEED_Y);
		isActive = true;
	}
}
