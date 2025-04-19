#pragma once
#include "GameObject.h"

class CCloud : public CGameObject
{
protected:
	int width;				// Unit: cell 

	const int idSprite[2][3] = {
		{ 1351, 1352, 1353 },
		{ 1354, 1355, 1356 }
	};

	int XYtoSpriteID(int x, int y)
	{
		return idSprite[x][CheckCondition(y, width + 2)];
	}

	int CheckCondition(int a, int b) {
		if (a == 0)
			return 0;
		else if (a > 0 && a < b - 1)
			return 1;
		else
			return 2;
	}
public:
	CCloud(float x, float y, int width) : CGameObject(x, y) {
		this->width = width;
		this->x = x;
		this->y = y;
	}
	void Render();
	void Update(DWORD dt) {};
	void GetBoundingBox(float& l, float& t, float& r, float& b) {};
};

