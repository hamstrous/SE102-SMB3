#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_COIN 11000

#define	COIN_WIDTH 10
#define COIN_BBOX_WIDTH 10
#define COIN_BBOX_HEIGHT 16


#define COIN_STATE_NORMAL 1
#define COIN_STATE_GONE 2
#define COIN_STATE_MOVEUP 3
#define COIN_STATE_MOVEDOWN 4
#define DISTANCE_COIN_MOVEUP 70
#define DISTANCE_COIN_MOVEDOWN 25
#define SPEED_COIN 0.3f

class CCoin : public CGameObject {
private:
	float startY;
public:
	CCoin(float x, float y) : CGameObject(x, y) {
		startY = y;
		SetState(COIN_STATE_NORMAL);
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(int state);
	int IsCollidable() { return 1; };
	int IsBlocking() { return 0; }
};