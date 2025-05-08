#pragma once
#include "GameObject.h"
#include "BaseBrick.h"
#include "GenericPlatform.h"

#define BRICK_BBOX_HEIGHT 16
#define BRICK_BBOX_WIDTH 16
class CUnbreakableBrick : public CBaseBrick
{
	int width;				// Unit: cell 
	int height;				// Unit: cell 
	int type; //	type: 0: wood, 1:blue 
	int brickSpriteId[2] = { 150001, 160001 };
public:
	int IsCollidable() { return 1; }
	int IsBlocking() { return 1; }
	CUnbreakableBrick(float x, float y, int type) : CBaseBrick(x, y) {
		this->type = type;
	}
	void GetBoundingBox(float& l, float& t, float& r, float& b) {
		l = x - BRICK_BBOX_WIDTH / 2;
		t = y - BRICK_BBOX_HEIGHT / 2;
		r = l + BRICK_BBOX_WIDTH;
		b = t + BRICK_BBOX_HEIGHT;
	}
	void Render();
	virtual void SetState(int state) {};
	virtual void SideHit() {};
	virtual void BottomHit() {};
};

