#include "Switch.h"
#include "Mario.h"

void CSwitch::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - SWITCH_BBOX / 2;
	top = y - SWITCH_BBOX / 2;
	right = left + SWITCH_BBOX;
	bottom = top + SWITCH_BBOX;
}

void CSwitch::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CSwitch::Render()
{	
	CAnimations* animations = CAnimations::GetInstance();
	if(off) animations->Get(ID_ANI_SWITCH_OFF)->Render(x, y);
	else animations->Get(ID_ANI_SWITCH_ON)->Render(x, y);
}

void CSwitch::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CMario*>(e->obj))
	{	
		//CGameFXManager::GetInstance()->AddGameFX(x, y, TYPE_SWITCH_SPAWN);
		off = true;
	}
		
}

void CSwitch::SetState(int state)
{
}

