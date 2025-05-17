#pragma once
#include "GameObject.h"


#define ID_ANI_HEAD 87001
#define ID_ANI_BODY 87002
#define ID_ANI_LEG  87003

#define BBOX_WIDTH 48
#define BBOX_HEIGHT 16

#define SPEED_Y	0.05f

class CMovingPlatform : public CGameObject
{
protected:
	bool isActive = false;
public:
	CMovingPlatform(float x, float y) :CGameObject(x, y) {}
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 1; }
	int IsCollidable() { return 1; }
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void OnCollisionWith(LPCOLLISIONEVENT e);
};

