#include "KoopaRed.h"
#include "debug.h"

CKoopaRed::CKoopaRed(float x, float y, bool hasWing) : CKoopa(x, y, hasWing)
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


void CKoopaRed::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CCharacter*>(e->obj)) {
		OnCollisionWithCharacter(e);
		return;
	}
	if (!e->obj->IsBlocking()) return;

	if (e->ny != 0 && !hasWing)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
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
		if (this->state == KOOPA_STATE_SHELL_IDLE) {
			y = (y + KOOPA_BBOX_HEIGHT_SHELL / 2) - KOOPA_BBOX_HEIGHT / 2; // when start walking, move up to normal y so dont drop through floor
			InitHorizontalSpeedBasedOnMario(KOOPA_WALKING_SPEED, -1);
			break;
		}
		vx = KOOPA_WALKING_SPEED * nx;
		// when start walking, walk toward mario
		break;
	case KOOPA_STATE_SHELL_MOVING:
		isIdle = false;
		InitHorizontalSpeedBasedOnMario(KOOPA_SHELL_SPEED); // when kicked, move away from mario
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
	case KOOPA_STATE_DIE:
		isDeleted = true;
		break;
	}
	CGameObject::SetState(state);

}

void CKoopaRed::Render()
{
	float shellTime = GetTickCount64() - shell_start;
	int aniId = 0;
	switch (state) {
	case KOOPA_STATE_FLYING:
		if (nx == 1) aniId = ID_ANI_KOOPA_WALKING_RIGHT;
		else aniId = ID_ANI_KOOPA_WALKING_LEFT;
		break;

	case KOOPA_STATE_WALKING:
		if (vx > 0) aniId = ID_ANI_KOOPA_WALKING_RIGHT;
		else if (vx < 0) aniId = ID_ANI_KOOPA_WALKING_LEFT;
		else aniId = ID_ANI_KOOPA_WALKING_RIGHT;
		break;
	case KOOPA_STATE_SHELL_HELD:
	case KOOPA_STATE_SHELL_IDLE:
		if (isIdle && shellTime > KOOPA_SHELL_COOLDOWN_VIBRATION && shellTime <= KOOPA_SHELL_COOLDOWN_VIBRATION_LEG) {
			aniId = ID_ANI_KOOPA_SHELL_VIBRATING;
		}
		else if (isIdle && GetTickCount64() - shell_start > KOOPA_SHELL_COOLDOWN_VIBRATION_LEG) {
			aniId = ID_ANI_KOOPA_SHELL_VIBRATING_LEG;
		}
		else {
			aniId = ID_ANI_KOOPA_SHELL_IDLE;
		}
		break;
	case KOOPA_STATE_SHELL_MOVING:
		aniId = ID_ANI_KOOPA_SHELL_MOVING;
		break;
	default:
		aniId = ID_ANI_KOOPA_WALKING_LEFT;
		break;
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	if (hasWing && nx == 1) CAnimations::GetInstance()->Get(ID_ANI_KOOPA_WING_RIGHT)->Render(x - 3, y - 3);
	else if (hasWing && nx == -1) CAnimations::GetInstance()->Get(ID_ANI_KOOPA_WING_LEFT)->Render(x + 3, y - 3);
	RenderBoundingBox();
}