#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "Mario.h"
#include "PlayScene.h"
#include "Coin.h"

#define ID_ANI_BLOCK_NORMAL 8000
#define ID_ANI_BLOCK_UNBOX 8001
#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 16

#define QUESTION_BLOCK_STATE_ITEM 1
#define QUESTION_BLOCK_STATE_MOVEUP 2
#define QUESTION_BLOCK_STATE_MOVEDOWN 3
#define QUESTION_BLOCK_STATE_UNBOX 4

#define SPEED_QUESTION_BLOCK 0.15f
#define DISTANCE_UP 7

#define ITEM_COIN 1
#define ITEM_LEVEL_UP 2
class CQuestionBlock : public CGameObject
{	
private:
	int type;
	float startY;
	bool isUnbox;
public:
	CQuestionBlock(float x, float y, int type) : CGameObject(x, y) {
		this->type = type;
		this->startY = y;
		this->isUnbox = false;
		state = QUESTION_BLOCK_STATE_ITEM;
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(int state);
	int IsCollidable() { return 1; };
	int IsBlocking() { return 1; };

};

