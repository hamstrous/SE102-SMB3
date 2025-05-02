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
#define BRICK_BBOX_WIDTH 14
#define BRICK_BBOX_HEIGHT 14

#define QUESTION_BLOCK_STATE_ITEM 1
#define QUESTION_BLOCK_STATE_MOVEUP 2
#define QUESTION_BLOCK_STATE_MOVEDOWN 3
#define QUESTION_BLOCK_STATE_UNBOX 4

#define SPEED_QUESTION_BLOCK 0.14f
#define DISTANCE_UP 16
#define DISTANCE_SPAWN 5

#define ITEM_COIN 1
#define ITEM_RED_MUSHROOM 2
#define ITEM_GREEN_MUSHROOM 3
#define ITEM_LEAF 4
class CQuestionBlock : public CBaseBrick
{	
private:
	int type;
	float startY;
	bool isUnbox;
public:
	CQuestionBlock(float x, float y, int type) : CBaseBrick(x, y) {
		this->type = type;
		this->startY = y;
		this->isUnbox = false;
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
	int IsBlocking() { return 1; };
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
};

