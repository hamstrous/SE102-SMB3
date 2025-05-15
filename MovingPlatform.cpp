#include "MovingPlatform.h"


void CMovingPlatform::Render()
{

}

void CMovingPlatform::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x + BBOX_WIDTH / 2;
	t = y - BBOX_HEIGHT / 2;
	r = l + BBOX_WIDTH;
	b = t + BBOX_HEIGHT;
}

void CMovingPlatform::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void CMovingPlatform::OnCollisionWith(LPCOLLISIONEVENT e)
{
}
