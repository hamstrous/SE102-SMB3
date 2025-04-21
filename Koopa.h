#pragma once
#include "GameObject.h"
#include "Character.h"
#include "Game.h"
#include "PlayScene.h"
#include "Scene.h"


#define KOOPA_GRAVITY 0.002f
#define KOOPA_WALKING_SPEED 0.05f
#define KOOPA_SHELL_SPEED 0.25f
#define KOOPA_SHELL_COOLDOWN 6000
#define KOOPA_SHELL_COOLDOWN_VIBRATION 4000
#define KOOPA_SHELL_COOLDOWN_VIBRATION_LEG 5000
#define KOOPA_FLYING_SPEED 0.04f
#define KOOPA_FLY_CHANGE_DIRECTION_TIME 1500


#define KOOPA_BBOX_WIDTH 16
#define KOOPA_BBOX_HEIGHT 26
#define KOOPA_BBOX_HEIGHT_SHELL 12

#define KOOPA_FLOOR_CHECK_BBOX_HEIGHT 4
#define KOOPA_FLOOR_CHECK_BBOX_WIDTH 4

#define KOOPA_SHELL_TIMEOUT 500

#define KOOPA_STATE_WALKING 100
#define KOOPA_STATE_DIE 200
#define KOOPA_STATE_SHELL_IDLE 300
#define KOOPA_STATE_SHELL_MOVING 400
#define KOOPA_STATE_SHELL_HELD 500
#define KOOPA_STATE_FLYING 600

//get hit by shell, tail
#define KOOPA_STATE_SHELL_HIT 700
#define KOOPA_STATE_FLYING_UP 0.3f

#define KOOPA_FLYING_SPEED_X 0.1f

class CKoopa : public CCharacter
{
protected:
	float ax;
	float ay;
	bool isCollidable = true;
	bool hasWing = true;

	bool isIdle = false;

	bool hit = false;
	ULONGLONG shell_start;
	ULONGLONG fly_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render() = 0;

	virtual int IsCollidable() { return isCollidable; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithCharacter(LPCOLLISIONEVENT e);


	virtual void Walking(DWORD dt, vector<LPGAMEOBJECT>* coObjects) = 0;
	virtual void Flying() = 0;

	void InitHorizontalSpeedBasedOnMario(float speed, float awayMario = 1);

public:
	CKoopa(float x, float y, bool hasWing);
	virtual void SetState(int state);
	virtual void Kicked();
	virtual void Stomped();
	virtual void Held();
	virtual void Release(); //Mario releases the shell
	virtual void ShellHit(int shellX);
	virtual void TailHit() {};
	virtual void BlockHit() {};
	virtual void Touched();
	virtual bool CanHold() { return state == KOOPA_STATE_SHELL_IDLE; };

};

