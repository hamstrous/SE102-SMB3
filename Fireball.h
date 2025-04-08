#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "Sprites.h"

#define FIREBALL_SPEED 0.07f
#define FIREBALL_TIME_DELETE 4000
#define FIREBALL_BBOX_WIDTH 8
#define FIREBALL_BBOX_HEIGHT 8

#define FIREBALL_STATE_MARIO 100
#define FIREBALL_STATE_PIRANHA 200
#define ID_ANI_FIREBALL 2500

class CFireball : public CGameObject
{
protected:
	ULONGLONG time_start;
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();

	int IsCollidable() { return 0; };
	int IsBlocking() { return 0; }
	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);
public:
	CFireball(float x, float y);
	void SetState(int state);
};

