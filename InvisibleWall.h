#pragma once
#include "BaseBrick.h"

#define INVISIBLE_WALL_STATE_MOVING 0
#define INVISIBLE_WALL_STATE_STATIC 1

#define INVISIBLE_WALL_SPEED 0.01f

class CInvisibleWall : public CBaseBrick
{
	float width;
	float height;
public:
	CInvisibleWall(float x, float y, float width, float height);

	void GetBoundingBox(float& l, float& t, float& r, float& b) {
		l = x - width/2;
		t = y - height/2;
		r = x + width;
		b = y + height;
	};

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();

	int IsCollidable() { return 1; }
	int IsBlocking() { return 1; }
};

