#pragma once
#include "GameObject.h"
#include "Koopa.h"
#include "Character.h"
#include "Game.h"
#include "PlayScene.h"
#include "Scene.h"
#include "ScoreManager.h"
#define KOOPA_FLYING_BOOST 0.5f

#define ID_ANI_KOOPA_WALKING_LEFT 20000
#define ID_ANI_KOOPA_WALKING_RIGHT 20001
#define ID_ANI_KOOPA_SHELL_IDLE 20002
#define ID_ANI_KOOPA_SHELL_MOVING 20003
#define ID_ANI_KOOPA_SHELL_MOVING_TAILHIT 20009
#define ID_ANI_KOOPA_SHELL_VIBRATING 20004
#define ID_ANI_KOOPA_SHELL_VIBRATING_LEG 20005
#define ID_ANI_KOOPA_WING_LEFT 20006
#define ID_ANI_KOOPA_WING_RIGHT 20007

#define ID_ANI_KOOPA_DIE_UP 20008
#define ID_ANI_KOOPA_SHELLHIT_VIBRATING	20009
#define ID_ANI_KOOPA_SHELLHIT_VIBRATING_LEG	20010

class CKoopaRed : public CKoopa
{
protected:
	CGameObject* FloorCheck;
	int count = 0;
	virtual void GetFloorBoundingBox(float& left, float& top, float& right, float& bottom);

	virtual int IsCollidable() { return KOOPA_STATE_SHELL_HELD != state; };
	virtual int IsBlocking() { return 0; }

	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithBaseBrick(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithCharacter(LPCOLLISIONEVENT e);

	virtual void Walking(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Flying();

	int OnFloor(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	virtual void Render();

public:
	CKoopaRed(float x, float y, bool hasWing);
	virtual void SetState(int state);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void Reset(const CCharacter* og) {
		*this = *(const CKoopaRed*)og;
		if (!hasWing) {
			SetState(KOOPA_STATE_WALKING);
			InitHorizontalSpeedBasedOnMario(KOOPA_WALKING_SPEED);
		}
		else {
			nx = -1;
			SetState(KOOPA_STATE_FLYING);
		}

	}

	CCharacter* Clone() {
		return new CKoopaRed(*this);
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
	virtual bool CanHold() { return state == KOOPA_STATE_SHELL_IDLE; };
	void HeldDie();
	void ThrownInBlock(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void ShellHeldTouch(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};



