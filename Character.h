#pragma once
#include "GameObject.h"

class CCharacter : public CGameObject
{
protected:
	float ax;	// acceleration on x-axis
	float ay;	// acceleration on y-axis
	float maxVx;
	float maxVy;
	float minVx;
	float minVy;
public:
	CCharacter(float x, float y) : CGameObject(x, y)
	{
		ax = 0.0f;
		ay = 0.0f;
		maxVx = 0.0f;
		maxVy = 0.0f;
		minVx = 0.0f;
		minVy = 0.0f;
	}
	virtual void Stomp();
	virtual void ShellHit();
	virtual void TailHit();
	virtual void BlockHit();
};

