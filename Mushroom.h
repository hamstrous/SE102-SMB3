#pragma once
#include "GameObject.h"
#include "PowerUp.h"
#define MUSHROOM_GRAVITY 0.0007f
#define MUSHROOM_SPEED 0.05f
#define MUSHROOM_SPEED_BOUNCING 0.07f
#define MUSHROOM_BOUNCING 0.2f


#define MUSHROOM_BBOX_WIDTH 14
#define MUSHROOM_BBOX_HEIGHT 14
#define MUSHROOM_BBOX_HEIGHT_PLUS 16

#define MUSHROOM_STATE_WALKING_RIGHT 100
#define MUSHROOM_STATE_WALKING_LEFT 150
#define MUSHROOM_STATE_UP 200
#define MUSHROOM_STATE_ONFLOOR 300
#define MUSHROOM_STATE_BOUNCING 400
#define MUSHROOM_STATE_BOUNCING_REVERSE 450
#define MUSHROOM_STATE_DELETE 500

#define ID_ANI_MUSHROOM_WALKING 8400
#define ID_ANI_MUSHROOM_1UP_WALKING 8500

#define TYPE_NORMAL 2
#define TYPE_LEVELUP 3
class CMushroom : public CPowerUp
{
protected:
	float ax;
	float ay;
	int type;
	float startY;
	bool dir;
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
	CMushroom(float x, float y, int type, bool dir)
	{
		this->x = x;
		this->y = y;
		this->type = type;
		ax = 0;
		ay = MUSHROOM_GRAVITY;
		SetState(MUSHROOM_STATE_WALKING_RIGHT);
		startY = y;
		this->dir = dir;
		time_start = GetTickCount64();
	}
	virtual void SetState(int state);
};

