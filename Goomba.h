#pragma once
#include "GameObject.h"
#include "Character.h"
#include "debug.h"


#define GOOMBA_GRAVITY 0.002f
#define GOOMBA_WALKING_SPEED 0.05f

#define GOOMBA_FLYING_SPEED 0.3f
#define GOOMBA_FLYING_SPEED_X 0.1f
#define GOOMBA_TAILHIT_SPEED_Y 0.5f

#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 14
#define GOOMBA_BBOX_HEIGHT_DIE 7

#define GOOMBA_DIE_TIMEOUT 500

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200
#define GOOMBA_STATE_DIE_UP 300


#define ID_ANI_GOOMBA_WALKING 5000
#define ID_ANI_GOOMBA_DIE 5001

class CGoomba : public CCharacter
{
protected:
	float ax;
	float ay;
	int dir;
	bool tailhit;
	ULONGLONG die_start;
	ULONGLONG die_up;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; }
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithCharacter(LPCOLLISIONEVENT e) {};

public:
	CGoomba(float x, float y);
	virtual void SetState(int state);
	virtual void Stomped();
	virtual void ShellHit(int shellX);
	virtual void TailHit(float x) {};
	virtual void BlockHit() {};
};