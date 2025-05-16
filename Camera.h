#pragma once
#include "GameObject.h"

#define CAMERA_SPEED 0.01f
#define FIXED_BOX_SIZE 20.0f
#define OUT_CAMERA 32.f

#define CAMERA_STATE_STATIC 0
#define CAMERA_STATE_MOVING 1
#define CAMERA_STATE_STOP 2

class CMario;

class CCamera : public CGameObject
{
	//camera position always the top left corner of the screen
	float levelWidth = 2816;
	float levelHeight = 626;
	//const float levelWidth = 3500; //2816 - 3500
	//const float levelHeight = 626;
	float screenWidth = 320;
	float screenHeight = 240;
public:
	CMario* GetMario();
	
	CCamera();
	CCamera(float x, float y);
	CCamera(float x, float y, float levelWidth, float levelHeight, float state);
	void Render() {};
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void SetState(int state) {
		CGameObject::SetState(state);
		switch (state)
		{
		case CAMERA_STATE_STATIC:
			vx = 0;
			break;
		case CAMERA_STATE_MOVING:
			vx = CAMERA_SPEED;
			break;
		case CAMERA_STATE_STOP:
			vx = 0;
			break;
		default:
			break;
		}
	}

	void UpdateStatic(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void UpdateMoving(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

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

