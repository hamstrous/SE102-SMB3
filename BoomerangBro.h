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
	CTimer *waitTimer = NULL, *jumpTimer = NULL, *throwTimer = NULL;
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

	 ~CBoomerangBro() {
		if (boomerang != NULL) {
			boomerang->Delete();
			boomerang = NULL;
		}
		if (waitTimer) {
			delete waitTimer;
			waitTimer = NULL;
		}

		if (throwTimer) {
			delete throwTimer;
			throwTimer = NULL;
		}

		if (jumpTimer) {
			delete jumpTimer;
			jumpTimer = NULL;
		}
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
		*this = CBoomerangBro(*(const CBoomerangBro*)og);
	}

	bool IsDead() {
		return state == BOOMERANG_BRO_STATE_DIE || state == BOOMERANG_BRO_STATE_DIE_UP;
	}

	CCharacter* Clone() { return new CBoomerangBro(*this); }

	CBoomerangBro(const CBoomerangBro& other) : CCharacter(other)
	{
		startX = other.startX;
		startY = other.startY;
		cycleCounter = other.cycleCounter;
		toLeft = other.toLeft;
		cycleChecker = other.cycleChecker;
		canThrow = other.canThrow;
		random = other.random;

		waitTimer = other.waitTimer ? new CTimer(*other.waitTimer) : nullptr;
		jumpTimer = other.jumpTimer ? new CTimer(*other.jumpTimer) : nullptr;
		throwTimer = other.throwTimer ? new CTimer(*other.throwTimer) : nullptr;

		// Do not copy boomerang ¨C keep it null to avoid duplicate rendering/logic
		boomerang = nullptr;

		state = other.state;
		x = other.x;
		y = other.y;
		vx = other.vx;
		vy = other.vy;
		nx = other.nx;
		ax = other.ax;
		ay = other.ay;
		isDeleted = other.isDeleted;
	}

	CBoomerangBro& operator=(const CBoomerangBro& other) {
		if (this == &other) return *this;

		CCharacter::operator=(other); // Copy base class

		startX = other.startX;
		startY = other.startY;
		cycleCounter = other.cycleCounter;
		toLeft = other.toLeft;
		cycleChecker = other.cycleChecker;
		canThrow = other.canThrow;
		random = other.random;

		// Clean up existing timers
		delete waitTimer;
		delete jumpTimer;
		delete throwTimer;

		waitTimer = other.waitTimer ? new CTimer(*other.waitTimer) : nullptr;
		jumpTimer = other.jumpTimer ? new CTimer(*other.jumpTimer) : nullptr;
		throwTimer = other.throwTimer ? new CTimer(*other.throwTimer) : nullptr;

		// Don't copy boomerang
		boomerang = nullptr;

		state = other.state;
		x = other.x;
		y = other.y;
		vx = other.vx;
		vy = other.vy;
		nx = other.nx;
		ax = other.ax;
		ay = other.ay;
		isDeleted = other.isDeleted;

		return *this;
	}

};

