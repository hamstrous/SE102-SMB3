#pragma once
#include "GameObject.h"
#include "Character.h"
#include "Animation.h"
#include "Animations.h"
#include "Sprites.h"
#include "Smoke.h"

#define PIRANHA_SPEED 0.04f
#define PIRANHA_BBOX 44
#define PRIRANHA_BBOX_WIDTH 15
#define PRIRANHA_BBOX_HEIGHT 30
#define PRIRANHA_STOP_TIMEOUT 1700

#define STATE_PRIRANHA_UP 100
#define STATE_PRIRANHA_STOP 200
#define STATE_PRIRANHA_DOWN 300

#define TAILHIT_DISTANCE1 368
#define TAILHIT_DISTANCE2 384

#define ID_ANI_STEM 7300
#define ID_ANI_PRIRANHA_RED_DOWN_LEFT 7311
#define ID_ANI_PRIRANHA_RED_DOWN_RIGHT 7313
#define ID_ANI_PRIRANHA_RED_DOWN_LEFT_STAY 7315
#define ID_ANI_PRIRANHA_RED_DOWN_RIGHT_STAY 7317
#define ID_ANI_PRIRANHA_RED_UP_LEFT 7312
#define ID_ANI_PRIRANHA_RED_UP_RIGHT 7314
#define ID_ANI_PRIRANHA_RED_UP_LEFT_STAY 7316
#define ID_ANI_PRIRANHA_RED_UP_RIGHT_STAY 7318


//get hit by shell, tail
#define STATE_PRIRANHA_SHELL_HIT 400


class CPlant : public CCharacter
{
protected:
	bool tailhit = false;
	ULONGLONG up_start;
	float startY;
	bool isFired;
	int color, type, size;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { 
		if (tailhit) return 1;
		else return 0; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);
	virtual void RenderBoundingBox();
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithCharacter(LPCOLLISIONEVENT e) {};

	bool GetIsFired() { return isFired; }
	void ShootFireball();
	int GetaniID();
public:
	CPlant(float x, float y, int corlor, int type, int size);
	virtual void SetState(int state);
	virtual void Stomped();

	void Reset(const CCharacter* og) override {
		*this = *(const CPlant*)og;
	}

	CCharacter* Clone() override {
		return new CPlant(*this);
	}


	virtual void ShellHit(int shellX);
	virtual void TailHit(float x);
	virtual void BlockHit() {};
};

