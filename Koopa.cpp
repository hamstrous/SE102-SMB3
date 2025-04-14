#include "Koopa.h"
#include "debug.h"

CKoopa::CKoopa(float x, float y)
{
	this->x = x;
	this->y = y;
	ax = 0;
	ay = KOOPA_GRAVITY;
	SetState(KOOPA_STATE_WALKING);
	InitHorizontalSpeed(KOOPA_WALKING_SPEED);
	shell_start = -1;
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

void CKoopa::InitHorizontalSpeed(float speed, float towardMario)
{
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	float mario_x, mario_y;
	scene->GetPlayer()->GetPosition(mario_x, mario_y);
	if (mario_x <= x) {
		vx = speed * towardMario;
	}
	else {
		vx = -speed * towardMario;
	}
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if(state == KOOPA_STATE_WALKING && OnFloor(dt, coObjects) == 1) {
		vx = -vx;
	}

	if((state == KOOPA_STATE_SHELL_IDLE || state == KOOPA_STATE_SHELL_HELD) && GetTickCount64() - shell_start > KOOPA_SHELL_COOLDOWN) {
		SetState(KOOPA_STATE_WALKING);
	}

	/*if ((state == GOOMBA_STATE_DIE) && (GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}*/

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
	DebugOutTitle(L"vx: %f, vy: %f", vx, vy);
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
		case KOOPA_STATE_SHELL_HELD:
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
	if (this->state == KOOPA_STATE_SHELL_HELD) {
		isCollidable = true; // when koopa is kicked, it can be collided with again
		ay = KOOPA_GRAVITY;
	}

	switch (state)
	{
	case KOOPA_STATE_SHELL_IDLE:
		if (this->state == KOOPA_STATE_WALKING) y = (y + KOOPA_BBOX_HEIGHT / 2) - KOOPA_BBOX_HEIGHT_SHELL / 2; // when start walking, move up to normal y so dont drop through floor
		// when start shell idle, move down to shell y so dont float above ground
		vx = 0;
		shell_start = GetTickCount64();
		break;
	case KOOPA_STATE_WALKING:
		shell_start = -1;
		Release(); //call to make sure shell is released (mario not holding)
		if(this->state == KOOPA_STATE_SHELL_IDLE) y = (y + KOOPA_BBOX_HEIGHT_SHELL / 2) - KOOPA_BBOX_HEIGHT / 2; // when start walking, move up to normal y so dont drop through floor
		InitHorizontalSpeed(KOOPA_WALKING_SPEED, -1); // when start walking, walk toward mario
		break;
	case KOOPA_STATE_SHELL_MOVING:
		shell_start = -1;
		InitHorizontalSpeed(KOOPA_SHELL_SPEED); // when kicked, move away from mario
		break;
	case KOOPA_STATE_SHELL_HELD:
		isCollidable = false;
		vx = 0;
		vy = 0;
		ay = 0;
		break;
	}
	CGameObject::SetState(state);

}

void CKoopa::Kicked()
{
	// Only when in shell state can Koopa be kicked by Mario
	if (state == KOOPA_STATE_SHELL_IDLE || state == KOOPA_STATE_SHELL_HELD) {
		SetState(KOOPA_STATE_SHELL_MOVING);
	}
}

void CKoopa::Held()
{
	if(state == KOOPA_STATE_SHELL_IDLE) SetState(KOOPA_STATE_SHELL_HELD);
}

void CKoopa::Release()
{
	if (state == KOOPA_STATE_SHELL_HELD) {
		CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		CMario* player = dynamic_cast<CMario*>(scene->GetPlayer());
		vy = -0.3;
		player->Drop();
	}
}
