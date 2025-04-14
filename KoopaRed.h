#pragma once
#include "GameObject.h"
#include "Koopa.h"
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
#define KOOPA_FLYING_SPEED 0.05f
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

	CGameObject* FloorCheck;

	virtual void GetFloorBoundingBox(float& left, float& top, float& right, float& bottom);

	virtual int IsCollidable() { return isCollidable; };
	virtual int IsBlocking() { return 0; }

	virtual void Walking(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Flying();

	int OnFloor(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

public:
	CKoopaRed(float x, float y);
	virtual void SetState(int state);
};



