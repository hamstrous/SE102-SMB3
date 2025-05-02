#pragma once
#include "GameObject.h"
class Camera : public CGameObject
{
	public:
	Camera(float x, float y) : CGameObject(x, y) {
		this->x = x;
		this->y = y;
	};
	void Render() {};
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};

