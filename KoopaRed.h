#pragma once
#include "GameObject.h"
#include "Koopa.h"
#include "Character.h"
#include "Game.h"
#include "PlayScene.h"
#include "Scene.h"

#define ID_ANI_KOOPA_WALKING_LEFT 20000
#define ID_ANI_KOOPA_WALKING_RIGHT 20001
#define ID_ANI_KOOPA_SHELL_IDLE 20002
#define ID_ANI_KOOPA_SHELL_MOVING 20003
#define ID_ANI_KOOPA_SHELL_VIBRATING 20004
#define ID_ANI_KOOPA_SHELL_VIBRATING_LEG 20005
#define ID_ANI_KOOPA_WING_LEFT 20006
#define ID_ANI_KOOPA_WING_RIGHT 20007

class CKoopaRed : public CKoopa
{
protected:
	CGameObject* FloorCheck;

	virtual void GetFloorBoundingBox(float& left, float& top, float& right, float& bottom);

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);


	virtual void Walking(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Flying();

	int OnFloor(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	virtual void Render();

public:
	CKoopaRed(float x, float y, bool hasWing);
	virtual void SetState(int state);
};



