#include "Koopa.h"
#include "debug.h"

CKoopa::CKoopa(float x, float y)
{
	this->x = x;
	this->y = y;
	ax = 0;
	vx = KOOPA_WALKING_SPEED;
	ay = KOOPA_GRAVITY;
	SetState(KOOPA_STATE_WALKING);
}

void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == KOOPA_STATE_WALKING)
	{
		left = x - KOOPA_BBOX_WIDTH / 2;
		top = y - KOOPA_BBOX_HEIGHT / 2;
		right = left + KOOPA_BBOX_WIDTH;
		bottom = top + KOOPA_BBOX_HEIGHT;
	}
	else {
		left = x - KOOPA_BBOX_WIDTH / 2;
		top = y - KOOPA_BBOX_HEIGHT_SHELL / 2;
		right = left + KOOPA_BBOX_WIDTH;
		bottom = top + KOOPA_BBOX_HEIGHT_SHELL;
	}
	
}

void CKoopa::GetFloorBoundingBox(float& left, float& top, float& right, float& bottom)
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

void CKoopa::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CKoopa*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

int CKoopa::OnFloor(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float ml, mt, mr, mb;
	GetFloorBoundingBox(ml, mt, mr, mb);
	return CCollision::GetInstance()->CheckStillTouchSolid(ml, mt, mr, mb, vx, vy, dt, coObjects);
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if(state != KOOPA_STATE_SHELL_MOVING && OnFloor(dt, coObjects) == 1) {
		vx = -vx;
	}

	/*if ((state == GOOMBA_STATE_DIE) && (GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}*/

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);

}


void CKoopa::Render()
{
	int aniId = 0;
	switch (state) {
		case KOOPA_STATE_WALKING:
			if (vx > 0) aniId = ID_ANI_KOOPA_WALKING_RIGHT;
			else if (vx < 0) aniId = ID_ANI_KOOPA_WALKING_LEFT;
			else aniId = ID_ANI_KOOPA_WALKING_RIGHT;
			break;
		case KOOPA_STATE_SHELL_IDLE:
			aniId = ID_ANI_KOOPA_SHELL_IDLE;
			break;
		case KOOPA_STATE_SHELL_MOVING:
			aniId = ID_ANI_KOOPA_SHELL_MOVING;
			break;
		default:
			aniId = ID_ANI_KOOPA_WALKING_RIGHT;
			break;
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void CKoopa::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPA_STATE_SHELL_IDLE:
		vx = 0;
		break;
	case KOOPA_STATE_WALKING:
		vx = KOOPA_WALKING_SPEED;
		break;
	}
	DebugOut(L"[INFO] Koopa state: %d\n", state);
}

void CKoopa::Kicked(float mx)
{
	// Only when in shell state can Koopa be kicked by Mario
	if (state == KOOPA_STATE_SHELL_IDLE) {
		SetState(KOOPA_STATE_SHELL_MOVING);
		if(mx <= x) {
			vx = KOOPA_SHELL_SPEED;
		}
		else{
			vx = -KOOPA_SHELL_SPEED;
		}
	}
}
