#pragma once

#include "GameObject.h"
#include "BaseBrick.h"
#include "Animation.h"
#include "Animations.h"
#include "Game.h"

#define ID_ANI_BREAKABLEBRICK 8100
#define ID_ANI_COIN_TYPE2 11002
#define ID_ANI_BOUNCING 8102

#define ID_ANI_BLOCK_UNBOX 8001
#define ID_ANI_BLOCK_UNBOX_BOUNCING 8103

#define BRICK_WIDTH 16
#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 16

#define STATE_MOVE_UP 100
#define STATE_MOVE_DOWN 200
#define DISTANCE_MOVE_UP 30

#define TYPE_ADDSCORE 2


class CBreakableBrick : public CBaseBrick {
protected:
	float start_y;
	
	bool unbox = false;
	bool unbox2 = false;
	int type = 0;
	ULONGLONG time_start = -1;
public:
	CBreakableBrick(float x, float y, int type = 0) : CBaseBrick(x, y) {
		start_y = y;
		this->x = x;
		this->y = y;
		this->type = type;
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { 
		if (CGame::GetInstance()->GetChangeBricktoCoin()) return 0;
		else return 1; };
	virtual void SetState(int state);
	virtual void SideHit();
	virtual void BottomHit();
	virtual void AddScoreBrick();
	virtual bool GetBouncing() { return bouncing; }
};