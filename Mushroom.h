#pragma once
#include "GameObject.h"
#define MUSHROOM_GRAVITY 0.0007f
#define MUSHROOM_SPEED 0.05f
#define MUSHROOM_SPEED_BOUNCING 0.1f
#define MUSHROOM_BOUNCING 0.3f


#define MUSHROOM_BBOX_WIDTH 16
#define MUSHROOM_BBOX_HEIGHT 16

#define MUSHROOM_STATE_WALKING 100
#define MUSHROOM_STATE_UP 200
#define MUSHROOM_STATE_ONFLOOR 300
#define MUSHROOM_STATE_BOUNCING 400

#define ID_ANI_MUSHROOM_WALKING 8400
#define ID_ANI_MUSHROOM_1UP_WALKING 8500

#define TYPE_NORMAL 2
#define TYPE_LEVELUP 3
class CMushroom : public CGameObject
{
protected:
	float ax;
	float ay;
	int type;
	float startY;
	ULONGLONG time_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void GetHaftBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	int OnFloor(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void InitHorizontalSpeed(float speed, float awayMario = 1);
public:
	CMushroom(float x, float y, int type) : CGameObject(x, y)
	{
		this->type = type;
		ax = 0;
		ay = MUSHROOM_GRAVITY;
		SetState(MUSHROOM_STATE_WALKING);
		startY = y;
		time_start = GetTickCount64();
	}
	virtual void SetState(int state);
};

