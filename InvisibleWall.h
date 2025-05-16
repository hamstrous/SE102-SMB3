#pragma once
#include "BaseBrick.h"

#define INVISIBLE_WALL_STATE_MOVING 1
#define INVISIBLE_WALL_STATE_STATIC 0

#define INVISIBLE_WALL_SPEED 0.01f

class CInvisibleWall : public CBaseBrick
{
	float width;
	float height;
	float endX;
public:
	CInvisibleWall(float x, float y, float width, float height);
	CInvisibleWall(float x, float y, float width, float height, float endX, int state);

	void GetBoundingBox(float& l, float& t, float& r, float& b) {
		l = x - width/2;
		t = y - height/2;
		r = x + width;
		b = y + height;
	};

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state) {
		CBaseBrick::SetState(state);
		switch (state)
		{
		case INVISIBLE_WALL_STATE_MOVING:
			vx = INVISIBLE_WALL_SPEED;
			break;
		case INVISIBLE_WALL_STATE_STATIC:
			vx = 0;
			break;
		default:
			break;
		}
	}

	int IsCollidable() { return 1; }
	int IsBlocking() { return 1; }
};

