#pragma once

#include <Windows.h>
#include <unordered_map>

#include "Sprites.h"
#include "AnimationFrame.h"

using namespace std;

class CAnimation
{
protected:
	ULONGLONG lastFrameTime;
	int defaultTime;
	int currentFrame;
	vector<LPANIMATION_FRAME> frames;
	bool done = false;
public:
	CAnimation(int defaultTime = 100) { this->defaultTime = defaultTime; lastFrameTime = -1; currentFrame = -1; }
	void Add(int spriteId, DWORD time = 0);
	virtual void Render(float x, float y);
	bool IsDone() { return done; }
	void Reset() { currentFrame = -1; done = false; }
};

typedef CAnimation* LPANIMATION;