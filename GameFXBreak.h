#pragma once
#include "GameFX.h"


class CGameFXBreak : public CGameFX
{
protected:
	float ax;
	float ay;
public:
	CGameFXBreak(float x, float y) : CGameFX(x,y,TYPE_BREAK){}
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);

};

