#pragma once
#include "GameObject.h"

#define CAMERA_SPEED 0.1f
#define FIXED_BOX_SIZE 20.0f
class CMario;

class CCamera : public CGameObject
{
	//camera position always the top left corner of the screen
	const float levelWidth = 2816;
	const float levelHeight = 626;
	float screenWidth = 320;
	float screenHeight = 240;
public:
	CMario* GetMario();
	
	CCamera();
	void Render() {};
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};

