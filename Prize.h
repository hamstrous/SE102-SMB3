#pragma once
#include "GameObject.h"
#include "Timer.h"

#define PRIZE_BBOX_WIDTH 16
#define PRIZE_BBOX_HEIGHT 16

#define PRIZE_CHANGE_TIME 100

#define PRIZE_UP_SPEED 0.15f

#define PRIZE_STATE_FLICKER 0
#define PRIZE_STATE_UP 1

class CPrize : public CGameObject
{
	// 0: star, 1: flower, 2: mushroom
	int type = 0;
	int spriteId[3] = {23100, 23101, 23102};
	int animationId[3] = {2310, 2311, 2312};
	CTimer* changeTimer = new CTimer(PRIZE_CHANGE_TIME);
public:
	CPrize(int x, int y) : CGameObject(x, y)
	{
		SetState(PRIZE_STATE_FLICKER);
	}
	~CPrize() {
		delete changeTimer;
		changeTimer = NULL;
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void SetState(int state);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	int IsCollidable() { return 1; };
	int IsBlocking() { return 0; };

};

