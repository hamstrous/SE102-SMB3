#include "GoombaFly.h"
#include "Goomba.h"
void CGoombaFly::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == GOOMBA_STATE_DIE || state == GOOMBA_STATE_DIE_UP)
	{
		left = top = right = bottom = 0;
	}
	else
	{
		left = x - GOOMBA_BBOX_WIDTH / 2;
		top = y - GOOMBA_BBOX_HEIGHT / 2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT;
	}
}

void CGoombaFly::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;
	if (((state == GOOMBAFLY_STATE_DIE) || (state == GOOMBAFLY_STATE_DIE_UP))
		&& (GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CGoombaFly::Render()
{
	int aniId = ID_ANI_GOOMBAFLY_WALKING;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	if (hasWing) 
	{
		CAnimations::GetInstance()->Get(ID_ANI_RIGHT_WING_WALKING)->Render(x - 6, y - 6);
		CAnimations::GetInstance()->Get(ID_ANI_LEFT_WING_WALKING)->Render(x + 6, y - 6);
	}
	if (state == GOOMBAFLY_STATE_DIE)
	{
		aniId = ID_ANI_GOOMBAFLY_DIE;
	}
	if (state == GOOMBAFLY_STATE_DIE_UP && dir > 0)
	{
		aniId = ID_ANI_GOOMBAFLY_DIE + 1;
	}
	if (state == GOOMBAFLY_STATE_DIE_UP && dir < 0)
	{
		aniId = ID_ANI_GOOMBAFLY_DIE + 2;
	}
	//RenderBoundingBox();
}

void CGoombaFly::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CGoombaFly::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CGoombaFly*>(e->obj)) return;
	if (dynamic_cast<CGoomba*>(e->obj)) return;
	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

CGoombaFly::CGoombaFly(float x, float y) : CCharacter(x, y)
{
	ax = 0;
	ay = GOOMBA_GRAVITY;
	tailhit = false;
	die_start = -1;
	hasWing = true;
	SetState(GOOMBAFLY_STATE_WALKING);
}

void CGoombaFly::SetState(int state)
{
	switch (state)
	{
	case GOOMBAFLY_STATE_DIE:
		if ((this->state == GOOMBAFLY_STATE_DIE) && (GetTickCount64() - die_start <= GOOMBA_DIE_TIMEOUT))
			break;
		// Set the die animation
		die_start = GetTickCount64();
		y += (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE) / 2;
		vx = 0;
		vy = 0;
		ay = 0;
		break;
	case GOOMBAFLY_STATE_WALKING:
		vx = -GOOMBA_WALKING_SPEED;
		break;
	case GOOMBAFLY_STATE_DIE_UP:
		die_start = GetTickCount64();
		/*if (tailhit) vy = -GOOMBAFLY_TAILHIT_SPEED_Y;
		else vy = -GOOMBAFLY_FLYING_SPEED;*/
		break;
	}
	CGameObject::SetState(state);
}

void CGoombaFly::Stomped()
{
	if (hasWing) hasWing = false;
	else SetState(GOOMBAFLY_STATE_DIE);
}

void CGoombaFly::ShellHit(int shellX)
{
}
