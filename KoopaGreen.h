#pragma once
#include "GameObject.h"
#include "Koopa.h"
#include "Character.h"
#include "Game.h"
#include "PlayScene.h"
#include "Scene.h"

#define KOOPA_FLYING_BOOST 0.3f
#define KOOPA_FLYING_WALKING_SPEED 0.031f

#define ID_ANI_KOOPA_WALKING_LEFT 20100
#define ID_ANI_KOOPA_WALKING_RIGHT 20101
#define ID_ANI_KOOPA_SHELL_IDLE 20102
#define ID_ANI_KOOPA_SHELL_MOVING 20103
#define ID_ANI_KOOPA_SHELL_MOVING_TAILHIT	201011
#define ID_ANI_KOOPA_SHELL_VIBRATING 20104
#define ID_ANI_KOOPA_SHELL_VIBRATING_LEG 20105
#define ID_ANI_KOOPA_WING_LEFT 20106
#define ID_ANI_KOOPA_WING_RIGHT 20107

#define ID_ANI_KOOPA_DIE_UP 20108
#define ID_ANI_KOOPA_SHELLHIT_VIBRATING	20109
#define ID_ANI_KOOPA_SHELLHIT_VIBRATING_LEG	201010

class CKoopaGreen : public CKoopa
{
protected:
	int count = 0;
	virtual void Walking(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Flying();
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithBaseBrick(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithCharacter(LPCOLLISIONEVENT e);
	
	virtual void Render();

public:
	CKoopaGreen(float x, float y, bool hasWing);
	virtual void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	void Reset(const CCharacter* og) {
		*this = *(const CKoopaGreen*)og;
		if (!hasWing) {
			SetState(KOOPA_STATE_WALKING);
			InitHorizontalSpeedBasedOnMario(KOOPA_WALKING_SPEED, -1);
		}
		else {
			SetState(KOOPA_STATE_FLYING);
		}
	}

	CCharacter* Clone() {
		return new CKoopaGreen(*this);
	}


	void InitHorizontalSpeedBasedOnMario(float speed, float awayMario = 1);
	virtual void Kicked();
	virtual void Stomped();
	virtual void Held();
	virtual void Release(bool dead); //Mario releases the shell
	virtual void ShellHit(int shellX);
	virtual void TailHit(float x);
	virtual void BlockHit() {};
	virtual void Touched();
	virtual bool CanHold() { return IsIdle(); };
	void HeldDie();
	void ThrownInBlock(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void ShellHeldTouch(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

};

