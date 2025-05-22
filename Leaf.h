#pragma once
#include "GameObject.h"
#include "PowerUp.h"
#include "Score.h"
#include "Mario.h"
#define LEAF_GRAVITY 0.00005f
#define LEAF_SPEED_UP 0.06f
#define LEAF_SPEED 0.08f

#define LEAF_BBOX_WIDTH 16
#define LEAF_BBOX_HEIGHT 14

#define LEAF_STATE_UP 100
#define LEAF_STATE_FALL 200
#define LEAF_STATE_STOP 300
#define LEAF_STATE_DELETE 400

#define ID_ANI_LEAF_RIGHT 8601
#define ID_ANI_LEAF_LEFT 8600

#define LEAF_UP_DISTANCE 26
#define LEAF_FALL_REVERSE_TIME 550

#define LEAF_TIME_STOP 10

class CLeaf : public CPowerUp
{
protected:
	float ax;
	float ay;
	float startY;
	ULONGLONG fallReverse_start;
	bool touch;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	int IsCollidable() {
		return 1;
	};
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CLeaf(float x, float y) {
		this->x = x;
		this->y = y;
		this->touch = false;
		ax = 0;
		ay = LEAF_GRAVITY;
		startY = y;
		killOffCam = true;
	}
	virtual void SetState(int state);
};

