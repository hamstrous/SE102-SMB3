#pragma once
#include "GameObject.h"

class CCharacter : public CGameObject
{
protected:
	bool frontRender = false;
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
	virtual ~CCharacter() {}	

	// clone to make a copy so that we can reinit when enemy reenter the camera
	virtual CCharacter* Clone() { return NULL; };
	virtual void Reset(const CCharacter* og) {};

	virtual void Stomped() = 0;
	virtual void ShellHit(int shellX) = 0;
	virtual void ShellHeldHit(int shellX) { //isDeleted = true;
		ShellHit(shellX);
	}; //hit while the shell is held
	virtual void TailHit(float x) = 0;
	virtual void BlockHit() = 0;
	virtual void Touched();
	virtual bool CanHold() { return false; };
	virtual bool ShellHeldTouched() { return false; };
	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() {
		return 0;
	}
	virtual void Ending();

	bool IsFrontRender() { return frontRender; }
};

