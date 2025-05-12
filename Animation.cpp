#include "Animation.h"
#include "Animations.h"
#include "Game.h"
#include "PlayScene.h"
#include "debug.h"

void CAnimation::Add(int spriteId, DWORD time)
{
	int t = time;
	if (time == 0) t = this->defaultTime;

	LPSPRITE sprite = CSprites::GetInstance()->Get(spriteId);
	if (sprite == NULL)
		DebugOut(L"[ERROR] Sprite ID %d not found!\n", spriteId);

	LPANIMATION_FRAME frame = new CAnimationFrame(sprite, t);
	frames.push_back(frame);
}

void CAnimation::Render(float x, float y, int mode)
{
	if (mode == 1 && type != 3) {
		StillRender(x, y);
		return;
	}

	if (type == 0)
		NormalRender(x, y);
	else if (type == 1)
		VibratingRender(x, y);
	else if (type == 2)
		FlickeringRender(x, y);
	else if (type == 3)
		StoppingRender(x, y);
	else if (type == 4)
		StoppingFlickeringRender(x, y);
	else if (type == 6)
		BouncingRender(x, y);
	else
		NormalRender(x, y);
}

void CAnimation::NormalRender(float x, float y)
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
			if (currentFrame == frames.size()) {
				done = 1;
				currentFrame = 0;
			}
		}

	}
	frames[currentFrame]->GetSprite()->Draw(x, y);
}

void CAnimation::VibratingRender(float x, float y)
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

void CAnimation::FlickeringRender(float x, float y)
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
			if (flickering) {
				flickering = false;
				currentFrame++;
				lastFrameTime = now;
				if (currentFrame == frames.size()) {
					done = 1;
					currentFrame = 0;
				}
			}
			else {
				flickering = true;
			}

		}

	}
	if (!flickering) frames[currentFrame]->GetSprite()->Draw(x, y);
}

void CAnimation::StoppingFlickeringRender(float x, float y)
{
	ULONGLONG now = GetTickCount64();
	if (currentFrame == -1)
	{
		dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->SetIsStop(FullTime());
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			if (flickering) {
				flickering = false;
				currentFrame++;
				lastFrameTime = now;
				if (currentFrame == frames.size()) {
					done = 1;
					currentFrame = 0;
				}
			}
			else {
				flickering = true;
			}

		}

	}
	if (!flickering) frames[currentFrame]->GetSprite()->Draw(x, y);
}

void CAnimation::StoppingRender(float x, float y)
{
	ULONGLONG now = GetTickCount64();
	if (currentFrame == -1)
	{
		dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->SetIsStop(FullTime());
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
			if (currentFrame == frames.size()) {
				done = 1;
				currentFrame = 0;
			}
		}

	}
	frames[currentFrame]->GetSprite()->Draw(x, y);
}

void CAnimation::StillRender(float x, float y)
{
	frames[(currentFrame != -1 ? currentFrame : 0)]->GetSprite()->Draw(x, y);
}

void CAnimation::BouncingRender(float x, float y)
{
	const float BOUNCE_MAX = 10.0f; 
	const float BOUNCE_SPEED = 1.5f; 
	ULONGLONG now = GetTickCount64();
	if (currentFrame == -1)
	{
		dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->SetIsStop(FullTime());
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
			if (currentFrame == frames.size()) {
				done = 1;
				currentFrame = 0;
			}
		}
		if (bouncingState == 0)
		{
			bouncingState = 1;
			bouncingOffset = 0.0f;
		}

		// Moving offset
		if (bouncingState == 1) // Going up
		{
			bouncingOffset += BOUNCE_SPEED;
			if (bouncingOffset >= BOUNCE_MAX)
			{
				bouncingOffset = BOUNCE_MAX;
				bouncingState = 2;
			}
		}
		else if (bouncingState == 2) // Going Down
		{
			bouncingOffset -= BOUNCE_SPEED;
			if (bouncingOffset <= 0.0f)
			{
				bouncingOffset = 0.0f;
				bouncingState = 0; // Done
			}
		}
	}
	

	frames[currentFrame]->GetSprite()->Draw(x, y - bouncingOffset);
}

