#pragma once
#include "GameObject.h"
#include "GenericPlatform.h"
#include "AssetIDs.h"
#define BRICK_BBOX_HEIGHT 16
#define BRICK_BBOX_WIDTH 16

class CCloudPlatform : public CGenericPlatform
{
protected:
	int aniId = 140001;
public:
	CCloudPlatform(float x, float y) : CGenericPlatform(x, y) {
		this->x = x;
		this->y = y;
	}
	void Render();
	void Update(DWORD dt) {};
	void GetBoundingBox(float& l, float& t, float& r, float& b)
	{
		l = x - BRICK_BBOX_WIDTH / 2;
		t = y - BRICK_BBOX_HEIGHT / 2;
		r = l + BRICK_BBOX_WIDTH;
		b = t + BRICK_BBOX_HEIGHT;
	}
	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 1; };
	int IsDirectionColliable(float nx, float ny) {
		if (nx == 0 && ny == -1) return 1;
		else return 0;
	}
};

