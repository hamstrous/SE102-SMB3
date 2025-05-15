#pragma once
#include "GameObject.h"


#define ID_ANI_HEAD 8700
#define ID_ANI_BODY 8701
#define ID_ANI_LEG  8702

#define BBOX_WIDTH 48
#define BBOX_HEIGHT 16

class CMovingPlatform : public CGameObject
{
protected:
public:
	CMovingPlatform(float x, float y) :CGameObject(x, y) {}
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 1; }
	int IsCollidable() { return 1; }
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void OnCollisionWith(LPCOLLISIONEVENT e);
};

