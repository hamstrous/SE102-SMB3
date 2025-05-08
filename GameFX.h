#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ANI_ID_TAILHIT 181000
#define ANI_ID_GAMEPAUSE 182000
#define ANI_ID_TIMEUP 183000

#define TIME_TAILHIT 150
#define TIME_TIMEUP 2000

#define TYPE_TAILHIT 1
#define TYPE_BREAK 2
#define TYPE_GAMEPAUSE 3
#define TYPE_TIMEUP 4

#define STATE_TIME_MOVEUP 100
#define STATE_TIME_STOP 200
#define STATE_OTHER 300

#define OFFSET_PAUSE_WIDTH 20
#define OFFSET_PAUSE_HEIGHT -110
class CGameFX : public CGameObject
{
protected:
	ULONGLONG start;
	int type;
	bool isOn = false;
	bool isOver = false;
public:
	CGameFX(float x, float y, int type) : CGameObject(x, y)
	{
		this->x = x;
		this->y = y;
		this->type = type;
		start = GetTickCount64();
		this->state = STATE_OTHER;
	}
	void SetOn(bool on) { isOn = on; }
	bool IsOn() { return isOn; }
	void Render();
	void Update(DWORD dt);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(int state);
	bool GetOver() { return isOver; }
};

