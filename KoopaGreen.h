#pragma once
#include "GameObject.h"
#include "Koopa.h"
#include "Character.h"
#include "Game.h"
#include "PlayScene.h"
#include "Scene.h"

#define KOOPA_FLYING_BOOST 0.5f

#define ID_ANI_KOOPA_WALKING_LEFT 20100
#define ID_ANI_KOOPA_WALKING_RIGHT 20101
#define ID_ANI_KOOPA_SHELL_IDLE 20102
#define ID_ANI_KOOPA_SHELL_MOVING 20103
#define ID_ANI_KOOPA_SHELL_VIBRATING 20104
#define ID_ANI_KOOPA_SHELL_VIBRATING_LEG 20105
#define ID_ANI_KOOPA_WING_LEFT 20106
#define ID_ANI_KOOPA_WING_RIGHT 20107

#define ID_ANI_KOOPA_DIE_UP 20108
class CKoopaGreen : public CKoopa
{
protected:
	virtual void Walking(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Flying();
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual void Render();

public:
	CKoopaGreen(float x, float y, bool hasWing);
	virtual void SetState(int state);

};

