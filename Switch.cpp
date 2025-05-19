#include "Switch.h"
#include "Mario.h"

void CSwitch::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - SWITCH_BBOX / 2;
	top = y - SWITCH_BBOX / 3;
	right = left + SWITCH_BBOX;
	bottom = top + SWITCH_BBOX;
}

void CSwitch::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->ProcessCollision(this, dt, coObjects);
	if(GetTickCount64() - time_start >= TIME_SWITCH)
		CGame::GetInstance()->SetChangeBricktoCoin(false);
}

void CSwitch::Render()
{	
	CAnimations* animations = CAnimations::GetInstance();
	if(off) animations->Get(ID_ANI_SWITCH_OFF)->Render(x, y);
	else animations->Get(ID_ANI_SWITCH_ON)->Render(x, y);
}

void CSwitch::OnCollisionWith(LPCOLLISIONEVENT e)
{	
	if (dynamic_cast<CMario*>(e->obj) && !off && e->nx == 0)
	{	
		off = true;
		CGame::GetInstance()->SetChangeBricktoCoin(true);
		time_start = GetTickCount64();
	}		
}

void CSwitch::SetState(int state)
{
}

void CSwitch::Enable()
{
	if (!off)
	{
		off = true;
		CGame::GetInstance()->SetChangeBricktoCoin(true);
		time_start = GetTickCount64();
	}
}

