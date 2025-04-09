#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "Sprites.h"

#define PIRANHA_SPEED 0.04f
#define PIRANHA_BBOX 50
#define PRIRANHA_BBOX_WIDTH 15
#define PRIRANHA_BBOX_HEIGHT 30
#define STEM_BBOX_HEIGHT 5
#define PRIRANHA_STOP_TIMEOUT 1700

#define STATE_PRIRANHA_UP 100
#define STATE_PRIRANHA_STOP 200
#define STATE_PRIRANHA_DOWN 300

#define ID_ANI_STEM 7300
#define ID_ANI_PRIRANHA_RED_DOWN_LEFT 7311
#define ID_ANI_PRIRANHA_RED_DOWN_RIGHT 7313
#define ID_ANI_PRIRANHA_RED_DOWN_LEFT_STAY 7315
#define ID_ANI_PRIRANHA_RED_DOWN_RIGHT_STAY 7317
#define ID_ANI_PRIRANHA_RED_UP_LEFT 7312
#define ID_ANI_PRIRANHA_RED_UP_RIGHT 7314
#define ID_ANI_PRIRANHA_RED_UP_LEFT_STAY 7316
#define ID_ANI_PRIRANHA_RED_UP_RIGHT_STAY 7318


class CPlant : public CGameObject
{
protected:
	int plant, stem;
	ULONGLONG up_start;
	float startY;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);
	virtual void RenderBoundingBox();
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithStem(LPCOLLISIONEVENT e);

	int GetaniID();
public:
	CPlant(float x, float y);
	virtual void SetState(int state);
};

