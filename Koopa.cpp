#include "Koopa.h"
#include "debug.h"

CKoopa::CKoopa(float x, float y)
{
	this->x = x;
	this->y = y;
	ax = 0;
	ay = KOOPA_GRAVITY;
	if (!hasWing) {
		SetState(KOOPA_STATE_WALKING);
		InitHorizontalSpeed(KOOPA_WALKING_SPEED);
	}
	else {
		nx = -1;
		SetState(KOOPA_STATE_FLYING);
	}
	
	isIdle = -1;
}

void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == KOOPA_STATE_WALKING || state == KOOPA_STATE_FLYING)
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
	if (hasWing)
		Flying();
	else
		Walking(dt, coObjects);
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
	DebugOutTitle(L"vx: %f, vy: %f", vx, vy);
}

void CKoopa::Render()
{
	float shellTime = GetTickCount64() - shell_start;
	int aniId = 0;
	switch (state) {
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
	if(hasWing && nx == 1) CAnimations::GetInstance()->Get(ID_ANI_KOOPA_WING_RIGHT)->Render(x - 3, y - 3);
	else if(hasWing && nx == -1) CAnimations::GetInstance()->Get(ID_ANI_KOOPA_WING_LEFT)->Render(x + 3, y - 3);
	RenderBoundingBox();
}

void CKoopa::SetState(int state)
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
		if(this->state == KOOPA_STATE_SHELL_IDLE) y = (y + KOOPA_BBOX_HEIGHT_SHELL / 2) - KOOPA_BBOX_HEIGHT / 2; // when start walking, move up to normal y so dont drop through floor
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

void CKoopa::Kicked()
{
	// Only when in shell state can Koopa be kicked by Mario
	if (state == KOOPA_STATE_SHELL_IDLE || state == KOOPA_STATE_SHELL_HELD) {
		SetState(KOOPA_STATE_SHELL_MOVING);
	}
}

void CKoopa::Stomped()
{
	if (state == KOOPA_STATE_FLYING) {
		hasWing = false;
		SetState(KOOPA_STATE_WALKING);
	}
	else if (state != KOOPA_STATE_SHELL_IDLE)
	{
		SetState(KOOPA_STATE_SHELL_IDLE);
	}
	else {
		Kicked();
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
