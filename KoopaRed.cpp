#include "KoopaRed.h"
#include "debug.h"

CKoopaRed::CKoopaRed(float x, float y) : CKoopa(x, y)
{
	
}

void CKoopaRed::GetFloorBoundingBox(float& left, float& top, float& right, float& bottom)
{
	top = y + KOOPA_BBOX_HEIGHT / 2;
	bottom = top + KOOPA_FLOOR_CHECK_BBOX_HEIGHT;
	if (vx > 0) {
		left = x + KOOPA_BBOX_WIDTH / 2;
		right = left + KOOPA_FLOOR_CHECK_BBOX_WIDTH;
	}
	else {
		right = x - KOOPA_BBOX_WIDTH / 2;
		left = right - KOOPA_FLOOR_CHECK_BBOX_WIDTH;
	}
}


int CKoopaRed::OnFloor(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float ml, mt, mr, mb;
	GetFloorBoundingBox(ml, mt, mr, mb);
	return CCollision::GetInstance()->CheckStillTouchSolid(ml, mt, mr, mb, vx, vy, dt, coObjects);
}


void CKoopaRed::Walking(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == KOOPA_STATE_WALKING && OnFloor(dt, coObjects) == 1) {
		vx = -vx;
	}

	if (vx > 0) nx = 1;
	else if (vx < 0) nx = -1;

	if (isIdle && GetTickCount64() - shell_start > KOOPA_SHELL_COOLDOWN) {
		SetState(KOOPA_STATE_WALKING);
	}
}
void CKoopaRed::Flying()
{
	if (GetTickCount64() - fly_start > KOOPA_FLY_CHANGE_DIRECTION_TIME) {
		vy = -vy;
		fly_start = GetTickCount64();
	}
}


void CKoopaRed::SetState(int state)
{
	if (this->state == KOOPA_STATE_SHELL_HELD) {
		isCollidable = true; // when koopa is kicked, it can be collided with again
		ay = KOOPA_GRAVITY;
	}
	else if (this->state == KOOPA_STATE_FLYING) {
		ay = KOOPA_GRAVITY;
	}

	switch (state)
	{
	case KOOPA_STATE_SHELL_IDLE:
		if (this->state == KOOPA_STATE_WALKING) y = (y + KOOPA_BBOX_HEIGHT / 2) - KOOPA_BBOX_HEIGHT_SHELL / 2; // when start walking, move up to normal y so dont drop through floor
		// when start shell idle, move down to shell y so dont float above ground
		vx = 0;
		shell_start = GetTickCount64();
		isIdle = true;
		break;
	case KOOPA_STATE_WALKING:
		isIdle = false;
		Release(); //call to make sure shell is released (mario not holding)
		if (this->state == KOOPA_STATE_SHELL_IDLE) y = (y + KOOPA_BBOX_HEIGHT_SHELL / 2) - KOOPA_BBOX_HEIGHT / 2; // when start walking, move up to normal y so dont drop through floor
		InitHorizontalSpeed(KOOPA_WALKING_SPEED, -1); // when start walking, walk toward mario
		break;
	case KOOPA_STATE_SHELL_MOVING:
		isIdle = false;
		InitHorizontalSpeed(KOOPA_SHELL_SPEED); // when kicked, move away from mario
		break;
	case KOOPA_STATE_SHELL_HELD:
		isCollidable = false;
		vx = 0;
		vy = 0;
		ay = 0;
		break;
	case KOOPA_STATE_FLYING:
		ay = 0;
		vy = KOOPA_FLYING_SPEED;
		fly_start = GetTickCount64();
		break;
	}
	CGameObject::SetState(state);

}


