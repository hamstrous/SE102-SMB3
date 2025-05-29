#pragma once
#include "GameFX.h"

#define GRAVITY 0.0007f
#define SPEED_X 0.08f
#define SPEED_X2 0.08f
#define SPEED_X3 0.05f
#define SPEED_Y 0.15f

#define LOW_JUMP 0.15f
#define HIGH_JUMP 0.3f
#define DOWN_LOW_JUMP 0.15f
#define DOWN_HIGH_JUMP 0.18f

#define STATE_LOW_JUMP 400
#define STATE_HIGH_JUMP 500
#define STATE_DOWN_LOW 600
#define STATE_DOWN_HIGH 700


#define TYPE_LEFT_TOP_BREAK 6
#define TYPE_LEFT_BOT_BREAK 7 
#define TYPE_RIGHT_TOP_BREAK 8
#define TYPE_RIGHT_BOT_BREAK 9

class CGameFXBreak : public CGameFX
{
protected:
	float ax;
	float ay;
public:
	CGameFXBreak(float x, float y, int type) : CGameFX(x, y, TYPE_BREAK) {
		this->ax = 0;
		this->ay = GRAVITY;
		this->type = type;
		if (type == TYPE_LEFT_BOT_BREAK) vx = -SPEED_X;
		else if (type == TYPE_LEFT_TOP_BREAK) vx = -SPEED_X;
		else if (type == TYPE_RIGHT_TOP_BREAK) vx = SPEED_X;
		else vx = SPEED_X;
		if(type == TYPE_LEFT_BOT_BREAK || type == TYPE_RIGHT_BOT_BREAK) SetState(STATE_LOW_JUMP);
		else SetState(STATE_HIGH_JUMP);
	}
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void SetState(int state);
	/*virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e, DWORD dt);*/
};

