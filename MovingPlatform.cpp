#include "MovingPlatform.h"
#include "Mario.h"
#include "Abyss.h"
#include "Game.h"
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
	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);

	float camWidth = CGame::GetInstance()->GetScreenWidth();
	float camHeight = CGame::GetInstance()->GetScreenHeight();

	if (!isActive) x += SPEED_X_MOVING_PLATFORM * dt;
	if (isActive) y += SPEED_Y_MOVING_PLATFORM * dt;

	/*if (isActive && (GetTickCount64() - delayTime >= TIME_DELAY) && noDelay) {
		y += SPEED_Y_MOVING_PLATFORM * dt;
		noDelay = false;
	}*/

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->ProcessCollision(this, dt, coObjects);
}

void CMovingPlatform::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CMario*>(e->obj) && e->ny > 0)
	{
		isActive = true;
		/*delayTime = GetTickCount64();
		noDelay = true;*/
	}
	if (dynamic_cast<CAbyss*>(e->obj)) isDeleted = true;
}
