#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "Score.h"
#include "PlayScene.h"
#define ID_ANI_COIN 11000
#define ID_ANI_COIN_2	11001

#define COIN_BBOX_WIDTH 16
#define COIN_BBOX_HEIGHT 16


#define COIN_STATE_NORMAL 0
#define COIN_STATE_GONE 1
#define COIN_STATE_HOLDUP 2
#define COIN_STATE_MOVEUP 3
#define COIN_STATE_MOVEDOWN 4
#define DISTANCE_COIN_MOVEUP 65
#define DISTANCE_COIN_MOVEDOWN 25
#define DISTANCE_COIN_HOLDUP 15
#define SPEED_COIN_FAST 0.15f
#define SPEED_COIN_SLOW 0.08f

#define TYPE_1	100
#define TYPE_2	200

class CCoin : public CGameObject {
private:
	float startY;
	int type;
public:
	CCoin(float x, float y, int type) : CGameObject(x, y) {
		startY = y;
		this->type = type;
		SetState(COIN_STATE_NORMAL);
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(int state);
	int IsCollidable() { return 1; };
	int IsBlocking() { return 0; }
};