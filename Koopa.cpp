#include "Koopa.h"
#include "debug.h"

CKoopa::CKoopa(float x, float y, bool hasWing) : CCharacter(x, y)
{
	this->x = x;
	this->y = y;
	ax = 0;
	ay = KOOPA_GRAVITY;

	this->hasWing = hasWing;

	if (!hasWing) {
		SetState(KOOPA_STATE_WALKING);
		InitHorizontalSpeedBasedOnMario(KOOPA_WALKING_SPEED);
	}
	else {
		nx = -1;
		SetState(KOOPA_STATE_FLYING);
	}
	
	isIdle = false;
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
	if (dynamic_cast<CCharacter*>(e->obj)) {
		OnCollisionWithCharacter(e);
		return;
	}
	if (!e->obj->IsBlocking()) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}

}

void CKoopa::OnCollisionWithCharacter(LPCOLLISIONEVENT e)
{
	CCharacter * character = dynamic_cast<CCharacter*>(e->obj);
	if (state == KOOPA_STATE_SHELL_MOVING) character->ShellHit(e->nx);
	else if (state == KOOPA_STATE_WALKING) vx = -vx;
}

void CKoopa::InitHorizontalSpeedBasedOnMario(float speed, float towardMario)
{
	// if towardMario == 0, then koopa will walk in the same direction as it is facing
	if (towardMario == 0) {
		vx = speed * nx;
		return;
	}
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
	if(vx > 0) nx = 1;
	else if(vx < 0) nx = -1;
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
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
		vy = -0.3f;
		player->Drop();
	}
}

void CKoopa::ShellHit(int shellX)
{
	SetState(KOOPA_STATE_DIE);
}

void CKoopa::Touched()
{
	CPlayScene* scene = (CPlayScene*)(CGame::GetInstance()->GetCurrentScene());
	CMario* mario = dynamic_cast<CMario*>(scene->GetPlayer());
	if (state != KOOPA_STATE_SHELL_IDLE) {
		mario->Attacked();
	}
	else {
		Kicked();
	}
}
