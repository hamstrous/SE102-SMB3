#pragma once
#include "Character.h"
#include "Random.h"
#include "Boomerang.h"

#define BOOMERANG_BRO_WALKING_SPEED 0.02f
#define BOOMERANG_BRO_JUMP_SPEED 0.25f
#define BOOMERANG_BRO_TAILHIT_SPEED 0.2f
#define BOOMERANG_BRO_GRAVITY 0.001f

#define BOOMERANG_WALK_LENGTH 32.f

#define BOOMERANG_BRO_WAIT_TIME 500
#define BOOMERANG_BRO_JUMP_TIME 1000
#define BOOMERANG_BRO_THROW_TIME 1500

#define BOOMERANG_BRO_BBOX_WIDTH 16
#define BOOMERANG_BRO_BBOX_HEIGHT 22

#define ID_ANI_BOOMERANG_BRO_WALKING_LEFT 3500
#define ID_ANI_BOOMERANG_BRO_WALKING_RIGHT 3501

#define ID_ANI_BOOMERANG_BRO_THROWING_LEFT 3502
#define ID_ANI_BOOMERANG_BRO_THROWING_RIGHT 3503

#define ID_ANI_BOOMERANG_BRO_DIE_LEFT 3504
#define ID_ANI_BOOMERANG_BRO_DIE_RIGHT 3505


#define BOOMERANG_BRO_STATE_WALKING 100
#define BOOMERANG_BRO_STATE_THROWING 200
#define BOOMERANG_BRO_STATE_JUMPING 300
#define BOOMERANG_BRO_STATE_DIE 400
#define BOOMERANG_BRO_STATE_DIE_UP 500

class CBoomerangBro : public CCharacter
{
	float startX, startY;
	int cycleCounter = 0;
	int toLeft = -1;
	bool cycleChecker = false; //to check the middwle of the cycle and count only one
	bool canThrow = false;
	CRandom random;
	CTimer *waitTimer, *jumpTimer, *throwTimer;
	CBoomerang* boomerang = NULL;
public:
	CBoomerangBro(float x, float y) : CCharacter(x, y)
	{
		startX = x;
		startY = y;
		SetState(BOOMERANG_BRO_STATE_WALKING);

		waitTimer = new CTimer(BOOMERANG_BRO_WAIT_TIME);
		throwTimer = new CTimer(BOOMERANG_BRO_THROW_TIME);
		jumpTimer = new CTimer(random.GetInt(8000,12000));
		jumpTimer->Start();
	}

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();

	void SetState(int state);


	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithCharacter(LPCOLLISIONEVENT e) {};
	void OnCollisionWithBoomerang(LPCOLLISIONEVENT e);
	
	void Stomped() {
		SetState(BOOMERANG_BRO_STATE_DIE);
	};
	void ShellHit(int shellX) {
		SetState(BOOMERANG_BRO_STATE_DIE);
		if (shellX > x) vx = -BOOMERANG_BRO_WALKING_SPEED;
		else vx = BOOMERANG_BRO_WALKING_SPEED;
	};
	void TailHit(float x) {
		SetState(BOOMERANG_BRO_STATE_DIE);
		if (x > this->x) vx = -BOOMERANG_BRO_WALKING_SPEED;
		else vx = BOOMERANG_BRO_WALKING_SPEED;
		vy = -BOOMERANG_BRO_TAILHIT_SPEED;
	};
	void BlockHit() {};

	void SetSleep(bool sleep) {

		CGameObject::SetSleep(sleep);
		if (boomerang != NULL && sleep) {
			boomerang->Delete();
			boomerang = NULL;
		}
	}

	void Reset(const CCharacter* og) {
		*this = *(const CBoomerangBro*) og;
	}

	bool IsDead() {
		return state == BOOMERANG_BRO_STATE_DIE || state == BOOMERANG_BRO_STATE_DIE_UP;
	}

	CCharacter* Clone() { return new CBoomerangBro(*this); }

};

