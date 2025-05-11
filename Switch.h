#pragma once
#include "GameObject.h"
#include "BaseBrick.h"
#include "PowerUp.h"
#define ID_ANI_SWITCH_ON 8200
#define ID_ANI_SWITCH_OFF 8201

#define SWITCH_BBOX 15

#define TIME_SWITCH 8000

class CSwitch : public CBaseBrick
{
protected:
	bool off = false;
	float startY;
	ULONGLONG time_start;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnCollision(LPCOLLISIONEVENT e);
public:
	CSwitch(float x, float y) : CBaseBrick(x, y) {};
	virtual void SetState(int state);
	void SetOff() { off = true; }
	void SideHit();
};



