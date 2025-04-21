#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_SMOKE 7350
#define SMOKE_TIME 800
#define SMOKE_STATE_DIE 100

class CSmoke : public CGameObject
{
	ULONGLONG start;
public:
	CSmoke(float x, float y) : CGameObject(x, y) {
		this->x = x;
		this->y = y;
		start = GetTickCount64();
	}
	void Render();
	void Update(DWORD dt);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(int state);
};

