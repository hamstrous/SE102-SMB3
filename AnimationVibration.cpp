#include "AnimationVibration.h"

void CAnimationVibration::Render(float x, float y)
{
	ULONGLONG now = GetTickCount64();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size()) currentFrame = 0;
		}

	}
	int offsetX = currentFrame % 2 == 0 ? 1 : -1;
	frames[currentFrame]->GetSprite()->Draw(x + offsetX * VIBRATING_SPEED, y);
}
