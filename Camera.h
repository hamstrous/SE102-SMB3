#pragma once
#include "GameObject.h"

#define CAMERA_SPEED 0.1f
#define FIXED_BOX_SIZE 20.0f
#define OUT_CAMERA 32.f
class CMario;

class CCamera : public CGameObject
{
	//camera position always the top left corner of the screen
	const float levelWidth = 3500; //2816 - 3500
	const float levelHeight = 626;
	float screenWidth = 320;
	float screenHeight = 240;
public:
	CMario* GetMario();
	
	CCamera();
	CCamera(float x, float y);
	void Render() {};
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	bool IsOutOfCamera(LPGAMEOBJECT obj) {
		float ox, oy;
		obj->GetPosition(ox, oy);
		if(ox < x - OUT_CAMERA || ox > x + screenWidth + OUT_CAMERA ||
			oy < y - OUT_CAMERA || oy > y + screenHeight + OUT_CAMERA)
			return true;
		else
			return false;
	}
};

