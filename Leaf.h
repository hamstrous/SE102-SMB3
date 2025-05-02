#pragma once
#include "GameObject.h"
#include "Score.h"
#define LEAF_GRAVITY 0.00002f
#define LEAF_SPEED_UP 0.1f
#define LEAF_SPEED 0.08f

#define LEAF_BBOX_WIDTH 16
#define LEAF_BBOX_HEIGHT 14

#define LEAF_STATE_UP 100
#define LEAF_STATE_FALL 200
#define LEAF_STATE_STOP 300
#define LEAF_STATE_DELETE 400

#define ID_ANI_LEAF_RIGHT 8601
#define ID_ANI_LEAF_LEFT 8600

#define LEAF_UP_DISTANCE 50
#define LEAF_FALL_REVERSE_TIME 300

#define LEAF_TIME_STOP 10

class CLeaf : public CGameObject
{
protected:
	float ax;
	float ay;
	float startY;
	ULONGLONG fallReverse_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 0; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CLeaf(float x, float y) {
		this->x = x;
		this->y = y;
		ax = 0;
		ay = LEAF_GRAVITY;
		startY = y;
	}
	virtual void SetState(int state);
};

