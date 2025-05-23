#pragma once
#include "GameObject.h"
#include "Character.h"
#include "Game.h"
#include "PlayScene.h"
#include "QuestionBlock.h"
#include "Scene.h"
#include "MovingPlatform.h"

#define KOOPA_GRAVITY 0.001f
#define KOOPA_WALKING_SPEED 0.03f
#define KOOPA_SHELL_SPEED 0.2f
#define KOOPA_SHELL_COOLDOWN 7000
#define KOOPA_SHELL_COOLDOWN_VIBRATION 4000
#define KOOPA_SHELL_COOLDOWN_VIBRATION_LEG 6000
#define KOOPA_FLYING_SPEED 0.04f
#define KOOPA_FLY_CHANGE_DIRECTION_TIME 1500


#define KOOPA_BBOX_WIDTH 16
#define KOOPA_BBOX_HEIGHT 24
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
#define KOOPA_STATE_TAILHIT 750
#define KOOPA_STATE_SHELL_HELD_TAILHIT 780
#define KOOPA_STATE_SHELL_MOVING_TAILHIT	790
#define KOOPA_FLYING_UP 0.3f
#define KOOPA_STATE_MARIO_DEAD 800

#define KOOPA_FLYING_SPEED_X 0.05f
#define KOOPA_FLYING_GRAVITY_Y 0.0001f //for shellheld release in sky

#define KOOPA_BOUNCE_SPEED 0.5f

#define KOOPA_TAILHIT_SPEED_Y 0.4f
#define KOOPA_TAILHIT_SPEED_x 0.02f

#define KOOPA_STATE_DIE_UP_ANI 800
#define KOOPA_STATE_DIE_UP 900
#define KOOPA_STATE_HIT_DIE 1000

#define KOOPA_TIME_DELETE 1000

//get hit by brick
#define KOOPA_STATE_BOUNCING 0.3f
class CKoopa : public CCharacter
{
protected:
	float ax;
	float ay;
	bool hasWing = true;

	bool isIdle = false;

	bool hit = false;

	bool bouncing = false;

	ULONGLONG shell_start;
	ULONGLONG fly_start;
	ULONGLONG delete_time;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) = 0;

	int IsCollidable() { return !IsHeld(); }
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt) = 0;

	virtual void OnCollisionWith(LPCOLLISIONEVENT e) = 0;
	virtual void OnCollisionWithCharacter(LPCOLLISIONEVENT e) = 0;
	virtual void OnCollisionWithBaseBrick(LPCOLLISIONEVENT e) = 0;

	virtual void Walking(DWORD dt, vector<LPGAMEOBJECT>* coObjects) = 0;
	virtual void Flying() = 0;

	virtual void InitHorizontalSpeedBasedOnMario(float speed, float awayMario = 1) = 0;

public:
	CKoopa(float x, float y) : CCharacter(x, y)
	{
		ax = 0;
		ay = KOOPA_GRAVITY;
		shell_start = -1;
		fly_start = -1;
	}
	virtual void SetState(int state) = 0;
	virtual void Kicked() = 0;
	virtual void Stomped() = 0;
	virtual void Held() = 0;
	virtual void Release(bool dead) = 0; //Mario releases the shell
	virtual void ShellHit(int shellX) = 0;
	virtual void TailHit(float x) = 0;
	virtual void BlockHit() = 0;
	virtual void Touched() = 0;
	virtual bool CanHold() { return state == KOOPA_STATE_SHELL_IDLE; };
	virtual void HeldDie() = 0;
	virtual void ThrownInBlock(DWORD dt, vector<LPGAMEOBJECT>* coObjects) = 0;
	virtual void ShellHeldTouch(DWORD dt, vector<LPGAMEOBJECT>* coObjects) = 0;


	bool IsIdle()
	{
		return state == KOOPA_STATE_SHELL_IDLE || state == KOOPA_STATE_TAILHIT;
	}

	bool IsHeld()
	{
		return state == KOOPA_STATE_SHELL_HELD || state == KOOPA_STATE_SHELL_HELD_TAILHIT;
	}

	bool IsUp() {
		return state == KOOPA_STATE_SHELL_HELD_TAILHIT || state == KOOPA_STATE_TAILHIT;
	}

	bool IsDown() {
		return state == KOOPA_STATE_SHELL_HELD || state == KOOPA_STATE_SHELL_IDLE;
	}

	bool IsMoving() {
		return state == KOOPA_STATE_SHELL_MOVING || state == KOOPA_STATE_SHELL_MOVING_TAILHIT;
	}
	
};

