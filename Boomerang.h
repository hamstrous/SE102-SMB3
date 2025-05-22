#pragma once

#include "GameObject.h"
#include "Timer.h"

#define BOOMERANG_VERTICAL_ACCE 0.0007f
#define BOOMERANG_HORIZONTAL_ACCE 0.0003f

#define BOOMERANG_HORIZONTAL_SPEED 0.12f
#define BOOMERANG_VERTICAL_SPEED 0.1f
#define BOOMERANG_MAX_VERTICAL_SPEED 0.24f

#define BOOMERANG_INITAL_UPWARD_SPEED 0.12f

#define BOOMERANG_GRAVITY 0.000225f//0.001125f
#define BOOMERANG_VERTICAL_DECEL 0.001125f

#define BOOMERANG_HORIZONTAL_LENGTH 64.f

#define BOOMERANG_BBOX_WIDTH 16
#define BOOMERANG_BBOX_HEIGHT 16

#define BOOMERANG_STATE_OUTWARD 100
#define BOOMERANG_STATE_RETURN 200
#define BOOMERANG_STATE_HOLDING 300

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
	float BOOMERANG_VERTICAL_LENGTH = 0.f;
public:

	CBoomerang(float x, float y) : CGameObject(x, y)
	{
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
		SetState(BOOMERANG_STATE_OUTWARD);
		killOffCam = true;
	}

	
};

