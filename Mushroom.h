#pragma once
#include "GameObject.h"
#define MUSHROOM_GRAVITY 0.0007f
#define MUSHROOM_SPEED 0.05f

#define MUSHROOM_BBOX_WIDTH 16
#define MUSHROOM_BBOX_HEIGHT 14

#define MUSHROOM_STATE_WALKING 100
#define MUSHROOM_STATE_UP 200

#define ID_ANI_MUSHROOM_WALKING 8400
#define ID_ANI_MUSHROOM_1UP_WALKING 8500

#define TYPE_NORMAL 1
#define TYPE_LEVELUP 2
class CMushroom : public CGameObject
{
protected:
	float ax;
	float ay;
	int type;
	ULONGLONG die_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CMushroom(float x, float y, int type) : CGameObject(x, y)
	{
		this->type = type;
		ax = 0;
		ay = MUSHROOM_GRAVITY;
		SetState(MUSHROOM_STATE_WALKING);
	}
	virtual void SetState(int state);
};

