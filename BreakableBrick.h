#pragma once

#include "GameObject.h"
#include "BaseBrick.h"
#include "Animation.h"
#include "Animations.h"
#include "Game.h"

#define ID_ANI_BREAKABLEBRICK 8100
#define ID_ANI_COIN_TYPE2 11002
#define ID_ANI_BOUNCING 8102

#define BRICK_WIDTH 16
#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 16

#define STATE_MOVE_UP 100
#define STATE_MOVE_DOWN 200
#define DISTANCE_MOVE_UP 30


class CBreakableBrick : public CBaseBrick {
protected:
	float start_y;
	bool bouncing = false;
	ULONGLONG time_start;
public:
	CBreakableBrick(float x, float y) : CBaseBrick(x, y) {
		start_y = y;
		this->x = x;
		this->y = y;
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { 
		if (CGame::GetInstance()->GetChangeBricktoCoin()) return 0;
		else return 1; };
	virtual void SetState(int state);
	virtual void SideHit();
	virtual void BottomHit();
};