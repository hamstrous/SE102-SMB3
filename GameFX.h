#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ANI_ID_TAILHIT 181000

#define TIME_TAILHIT 150

#define TYPE_TAILHIT 1
#define TYPE_BREAK 2

class CGameFX : public CGameObject
{
protected:
	ULONGLONG start;
	int type;
public:
	CGameFX(float x, float y, int type) : CGameObject(x, y)
	{
		this->x = x;
		this->y = y;
		this->type = type;
		start = GetTickCount64();
	}
	void Render();
	void Update(DWORD dt);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(int state);
};

