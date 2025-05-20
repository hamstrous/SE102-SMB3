#pragma once
#include "GameObject.h"

#define TYPE_SMALL 0
#define TYPE_BIG   1

#define ID_ANI_SMALL_CLOUD     1360
#define ID_ANI_LEFT_BIG_CLOUD  1358
#define ID_ANI_RIGHT_BIG_CLOUD 1359

class CCloudEllipse : public CGameObject
{
protected:
	int type;
public:
	CCloudEllipse(float x, float y, int type) : CGameObject(x, y){
		this->type = type;
	}

	void GetBoundingBox(float& l, float& t, float& r, float& b) {
		l = t = r = b = 0;
	}

	void Render();
};

