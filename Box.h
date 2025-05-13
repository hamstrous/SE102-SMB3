#pragma once
#include "GameObject.h"

class CBox : public CGameObject
{
	float l, r, t, b;
public:
	CBox(float l, float t, float r, float b) : CGameObject() {
		this->l = l;
		this->t = t;
		this->r = r;
		this->b = b;
	}
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) {
		left = l; top = t; right = r; bottom = b;
	};
	float GetBoundingBoxTop() { return t; }
	float GetBoundingBoxBottom() { return b; }
	float GetBoundingBoxLeft() { return l; }
	float GetBoundingBoxRight() { return r; }
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {}
	void Render() {
		//RenderBoundingBox();
	};
};

