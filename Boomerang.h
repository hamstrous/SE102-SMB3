#pragma once

#include "GameObject.h"
#include "Timer.h"

#define BOOMERANG_VERTICAL_ACCE 0.0007f
#define BOOMERANG_HORIZONTAL_ACCE 0.0003f

#define BOOMERANG_FLYING_SPEED 0.2f
#define BOOMERANG_VERTICAL_SPEED 0.1

#define BOOMERANG_VERTICAL_LENGTH 56.f
#define BOOMERANG_HORIZONTAL_LENGTH 64.f

#define BOOMERANG_BBOX_WIDTH 16
#define BOOMERANG_BBOX_HEIGHT 16

#define BOOMERANG_STATE_FLYING_UP 100
#define BOOMERANG_STATE_FLYING_DOWN 200
#define BOOMERANG_STATE_MOVING_UP 300
#define BOOMERANG_STATE_MOVING_DOWN 400
#define BOOMERANG_STATE_HOLDING 500

#define ID_ANI_BOOMERANG_FLYING_RIGHT 3600
#define ID_ANI_BOOMERANG_FLYING_LEFT 3601
#define ID_ANI_BOOMERANG_HOLDING_RIGHT 3602
#define ID_ANI_BOOMERANG_HOLDING_LEFT 3603

using namespace std;

class CBoomerang : public CGameObject
{
	float ay = 0;
	float ax = 0;
	float startY = 0;
	float startX = 0;
	CTimer * upTimer;
public:

	CBoomerang(float x, float y) : CGameObject(x, y)
	{
		startY = y;
		startX = x;
		upTimer = new CTimer(1000);
		upTimer->Start();
		killOffCam = false;
	}

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithBoomerangBro(LPCOLLISIONEVENT e) { Delete(); };

	int IsCollidable() { return 1; };
	int IsBlocking() { return 0; };

	void SetHold(int x, int y, int nx)
	{
		SetPosition(x, y);
		this->nx = nx;
		SetState(BOOMERANG_STATE_HOLDING);
	}

	void SetStartFlying(int nx)
	{
		SetState(BOOMERANG_STATE_MOVING_UP);
		killOffCam = true;
	}

};

