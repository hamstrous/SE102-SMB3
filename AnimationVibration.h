#pragma once

#pragma once

#include <Windows.h>
#include <unordered_map>

#include "Sprites.h"
#include "AnimationFrame.h"
#include "Animation.h"

using namespace std;


class CAnimationVibration : public CAnimation
{
	const float VIBRATING_SPEED = 0.5f;
public:
	CAnimationVibration(int defaultTime = 100) : CAnimation(defaultTime) {}
	void Render(float x, float y);
};
typedef CAnimationVibration* LPANIMATION_VIBRATION;

