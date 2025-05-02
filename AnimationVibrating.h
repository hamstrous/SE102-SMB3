#pragma once

#pragma once

#include <Windows.h>
#include <unordered_map>

#include "Sprites.h"
#include "AnimationFrame.h"
#include "Animation.h"

using namespace std;


class CAnimationVibrating : public CAnimation
{
	const float VIBRATING_SPEED = 0.5f;
public:
	CAnimationVibrating(int defaultTime = 100) : CAnimation(defaultTime) {}
	void Render(float x, float y);
};
typedef CAnimationVibrating* LPANIMATION_VIBRATING;

