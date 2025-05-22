#pragma once
#include "GameObject.h"
class CBaseBrick : public CGameObject
{
public:
	CBaseBrick(float x, float y) : CGameObject(x, y) {}
	virtual void Render() {};
	virtual void Update(DWORD dt) {};
	void GetBoundingBox(float& l, float& t, float& r, float& b) {};
	virtual int IsCollidable() { return 1; }
	virtual int IsBlocking() { return 1; }
	virtual void SideHit() {};
	virtual void BottomHit() {};
	virtual bool GetBouncing() { return 0; }
};

