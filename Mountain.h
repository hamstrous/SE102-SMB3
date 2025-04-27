#pragma once
#include "GameObject.h"
#include "debug.h"

class CMountain : public CGameObject
{
protected:			// Unit: cell 
	float width;
	float height;
	int type; //0: body, 1:head
	int color; //0: green, 1: black

	const int mountainSpriteId[2][2][3] = {
		{
			{
				100003, 100005, 100004
			},
			{
				110003, 110005, 110004
			}
		},
		{
			{
				100001, 0, 100002
			},
			{
				110001, 0, 110002
			}
		}
	};

	int XYtoSpriteID(int x, int y,int type, int color)
	{
		return mountainSpriteId[type][color][CheckCondition(y, width)];
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
	void Render();
	void Update(DWORD dt) {}
	CMountain(float x, float y, int width, int height,int type, int color);
};

