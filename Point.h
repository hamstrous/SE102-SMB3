#pragma once
#include "GameObject.h"

// This class is for mario collision detection point
class CPoint : public CGameObject
{
	float beforeBlockX;
	float beforeBlockY;
public:
	CPoint(float x, float y) : CGameObject(x, y) {
		this->beforeBlockX = x;
		this->beforeBlockY = y;
	};

	void Update(DWORD dt, std::vector<LPGAMEOBJECT>* coObjects = nullptr) {};
	void Render() {};
	int IsBlocking() { return false; };
	int IsCollidable() { return true; };
	void GetBoundingBox(float& l, float& t, float& r, float& b) {
		l = r = x;
		t = b = y;
	};

	void SetBeforeBlockPosition(float x, float y) {
		this->beforeBlockX = x;
		this->beforeBlockY = y;
	};

	void GetBeforeBlockPosition(float& x, float& y) {
		x = beforeBlockX;
		y = beforeBlockY;
	};

	void RenderBoundingBox();
};

