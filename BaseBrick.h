#pragma once
#include "GameObject.h"
#include "Timer.h"
#define INVINCIBLE_TIME_FROM_TAIL_HIT 500 // 2 seconds
class CBaseBrick : public CGameObject
{
protected:
	CTimer* invincibleTimer = NULL;
	bool bouncing = false;
public:
	CBaseBrick(float x, float y) : CGameObject(x, y) {
		invincibleTimer = new CTimer(INVINCIBLE_TIME_FROM_TAIL_HIT);
	}
	~CBaseBrick() {
		if(invincibleTimer != NULL) delete invincibleTimer;
	}
	virtual void Render() {};
	virtual void Update(DWORD dt) {};
	void GetBoundingBox(float& l, float& t, float& r, float& b) {};
	virtual int IsCollidable() { return 1; }
	virtual int IsBlocking() { return 1; }
	virtual void SideHit() {};
	virtual void BottomHit() {};
	virtual bool GetBouncing() { return 0; }
};

