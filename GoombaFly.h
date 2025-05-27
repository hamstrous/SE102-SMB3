#pragma once
#include "GameObject.h"
#include "Character.h"
#include "debug.h"

#define GOOMBA_GRAVITY 0.002f
#define GOOMBA_JUMP_GRAVITY 0.001f
#define GOOMBA_WALKING_SPEED 0.03f
#define GOOMBA_SMALL_JUMP_SPEED 0.15f
#define GOOMBA_FLYING_SPEED 0.35f

#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 14
#define GOOMBA_BBOX_HEIGHT_DIE 7

#define GOOMBAFLY_DIE_TIMEOUT 900
#define GOOMBAFLY_DIE_UP_TIMEOUT 2000


#define GOOMBAFLY_STATE_WALKING 100
#define GOOMBAFLY_STATE_DIE 300
#define GOOMBAFLY_STATE_DIE_UP 400
#define GOOMBAFLY_STATE_SMALL_JUMP 500
#define GOOMBAFLY_STATE_BIG_JUMP 600

#define ID_ANI_GOOMBAFLY_WALKING 5100
#define ID_ANI_GOOMBAFLY_DIE 5101

#define ID_ANI_GOOMBAFLY_DIE_UP_LEFT 5102
#define ID_ANI_GOOMBAFLY_DIE_UP_RIGHT 5103

#define ID_ANI_RIGHT_WING_WALKING 5201
#define ID_ANI_LEFT_WING_WALKING 5200
#define ID_ANI_RIGHT_WING_SMALL_JUMP 5203
#define ID_ANI_LEFT_WING_SMALL_JUMP 5202
#define ID_ANI_RIGHT_WING_BIG_JUMP 5205
#define ID_ANI_LEFT_WING_BIG_JUMP 5204

#define WALKING_TIME 600
#define SMALL_JUMP_TIME 800
#define BIG_JUMP_TIME 500
#define TOWARD_MARIO_TIME 300
class CGoombaFly : public CCharacter
{
protected:
	float ax;
	float ay;
	int dir;
	bool tailhit;
	bool hasWing;
	ULONGLONG die_start;
	ULONGLONG die_up;
	ULONGLONG walking_start;
	ULONGLONG small_jump_start;
	ULONGLONG big_jump_start;
	ULONGLONG toward_mario_start;
	ULONGLONG start_small_jump;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; }
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithCharacter(LPCOLLISIONEVENT e) {};

public:
	CGoombaFly(float x, float y);
	virtual void SetState(int state);
	virtual void Stomped();
	virtual void ShellHit(int shellX);
	virtual void TailHit(float x);
	virtual void BlockHit() {};
	void TowardMario(float speed, float awayMario = 1);

	void Reset(const CCharacter* og) override {
		*this = *(const CGoombaFly*)og;
	}

	CCharacter* Clone() override {
		return new CGoombaFly(*this);
	}

};

