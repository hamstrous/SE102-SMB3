#pragma once
#include "GameObject.h"

#define KOOPA_GRAVITY 0.002f
#define KOOPA_WALKING_SPEED 0.05f


#define KOOPA_BBOX_WIDTH 16
#define KOOPA_BBOX_HEIGHT 26
#define KOOPA_BBOX_HEIGHT_DIE 7

#define KOOPA_DIE_TIMEOUT 500

#define KOOPA_STATE_WALKING 100
#define KOOPA_STATE_DIE 200

#define ID_ANI_KOOPA_WALKING_LEFT 20000
#define ID_ANI_KOOPA_WALKING_RIGHT 20001
#define ID_ANI_KOOPA_DIE 20002

class CKoopa : public CGameObject
{
	float ax;
	float ay;

	ULONGLONG die_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CKoopa(float x, float y);
	virtual void SetState(int state);
};

