#pragma once

#include <Windows.h>
#include <unordered_map>

#include "Sprites.h"
#include "AnimationFrame.h"

using namespace std;

class CAnimation
{
	const float VIBRATING_SPEED = 0.5f;

protected:
	ULONGLONG lastFrameTime;
	int defaultTime;
	int currentFrame;
	vector<LPANIMATION_FRAME> frames;
	bool done = false;
	int doneTime = 0;
	bool flickering = false;
	//0:normal, 1:vibrate, 2:flicker
	int type = 0;
public:
	CAnimation(int defaultTime = 100, int type = 0) 
	{ 
		this->defaultTime = defaultTime; 
		lastFrameTime = -1; 
		currentFrame = -1; 
		this->type = type; 
	}
	void Add(int spriteId, DWORD time = 0);
	virtual void Render(float x, float y);
	virtual void NormalRender(float x, float y);
	virtual void VibratingRender(float x, float y);
	virtual void FlickeringRender(float x, float y);
	void SetType(int type) { this->type = type; }
	bool IsDone() { return done; }
	void Reset() { currentFrame = -1; done = false; }
	int FullTime()
	{
		if(doneTime != 0) return doneTime; // Return the animation time if it has been set
		int t = 0;
		for (int i = 0; i < frames.size(); i++)
			t += frames[i]->GetTime();
		return t;
	}
};

typedef CAnimation* LPANIMATION;