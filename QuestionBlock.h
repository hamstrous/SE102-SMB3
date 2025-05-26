#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "Mario.h"
#include "PlayScene.h"
#include "Coin.h"
#include "Mushroom.h"
#include "Leaf.h"
#include "debug.h"
#include "BaseBrick.h"
#define ID_ANI_BLOCK_NORMAL 8000
#define ID_ANI_BLOCK_UNBOX 8001
#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 16

#define ID_ANI_BREAKABLEBRICK 8100

#define QUESTION_BLOCK_STATE_ITEM 100
#define QUESTION_BLOCK_STATE_MOVEUP 200
#define QUESTION_BLOCK_STATE_MOVEDOWN 300
#define QUESTION_BLOCK_STATE_UNBOX 400

#define SPEED_QUESTION_BLOCK 0.14f
#define DISTANCE_UP 16
#define DISTANCE_SPAWN 5

#define ITEM_COIN 1
#define ITEM_RED_MUSHROOM 2
#define ITEM_GREEN_MUSHROOM 3
#define ITEM_SWITCH 4
#define ITEM_BREAK_LEVEL_UP 5

#define TIME_UP 130
class CQuestionBlock : public CBaseBrick
{	
private:
	int type;
	float startY;
	bool isUnbox;
	bool up;
	ULONGLONG start_up = -1;
	ULONGLONG stillUnbox = -1;
public:
	CQuestionBlock(float x, float y, int type) : CBaseBrick(x, y) {
		this->type = type;
		this->startY = y;
		this->isUnbox = false;
		up = false;
		state = QUESTION_BLOCK_STATE_ITEM;
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(int state);
	int IsCollidable() {
		return 1;
	};
	bool Hit() { return isUnbox; };
	int IsBlocking() {
		if (/*state == QUESTION_BLOCK_STATE_MOVEUP ||*/ state == QUESTION_BLOCK_STATE_MOVEDOWN && type >=3)
			return 0;
		return 1;
	};
	void OnCollisionWith(LPCOLLISIONEVENT e);
	float ReturnXmin() {
		float l;
		l = x - BRICK_BBOX_WIDTH / 2;
		return l;
	};
	float ReturnXmax() {
		float r;
		r = x + BRICK_BBOX_WIDTH / 2;
		return r;
	};
	bool getUp() { return up; };
	float getY() { return startY; };
	void SideHit();
	void BottomHit();

	bool GetBouncing() { return bouncing; };
};

