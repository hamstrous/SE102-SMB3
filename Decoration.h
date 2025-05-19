#pragma once
#include "GameObject.h"
// 0: Blue, 1: Black, 2:mushroom, 3:flower, 4:star, 5:smallbush, 6:halfblack
class CDecoration : public CGameObject
{
	int type; //0:mushroom, 1:flower, 2:star, 3:smallbush, 4: border
	int colorSpriteId[5] = { 170001, 170002, 170003, 130001, 170006 };
public:
	CDecoration(float x, float y, int type) : CGameObject(x, y) {
		this->type = type;
	}
	void GetBoundingBox(float& l, float& t, float& r, float& b) {
		l = t = r = b = 0;
	}
	void Render();
};

