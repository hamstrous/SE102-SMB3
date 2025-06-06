#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ANI_ID_TAILHIT 181000
#define ANI_ID_GAMEPAUSE 182000
#define ANI_ID_TIMEUP 183000
#define ANI_ID_SWITCH_SPAWN 184000
#define ANI_ID_BREAKABLEBRICK 185000

#define TIME_TAILHIT 150
#define TIME_TIMEUP 5000
#define TIME_SWITCH_SPAWN 800
#define TIME_BREAK 2000

#define TYPE_TAILHIT 1
#define TYPE_BREAK 2
#define TYPE_GAMEPAUSE 3
#define TYPE_TIMEUP 4
#define TYPE_SWITCH_SPAWN 5



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
	}
	void SetOn(bool on) { isOn = on; }
	bool IsOn() { return isOn; }
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
	virtual bool GetOver() { return isOver; }
	virtual void SetOver() {  isOver = true; }
};

