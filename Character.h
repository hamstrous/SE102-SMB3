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
	virtual void OnCollisionWithCharacter(LPCOLLISIONEVENT e) = 0;
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

	virtual void Stomped() = 0;
	virtual void ShellHit(int shellX) = 0;
	virtual void TailHit() = 0;
	virtual void BlockHit() = 0;
	virtual void Touched();
	virtual bool CanHold() { return false; };
};

