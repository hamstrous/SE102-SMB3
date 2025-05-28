#include "GoombaFly.h"
#include "Goomba.h"
#include "PlayScene.h"
#include "Mario.h"
void CGoombaFly::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == GOOMBAFLY_STATE_DIE || state == GOOMBAFLY_STATE_DIE_UP)
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

	vy = min(vy, 0.2f);

	if ( (state == GOOMBAFLY_STATE_DIE) && (GetTickCount64() - die_start > GOOMBAFLY_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}
	else if ((state == GOOMBAFLY_STATE_DIE_UP) && (GetTickCount64() - die_start > GOOMBAFLY_DIE_UP_TIMEOUT))
	{
		isDeleted = true;
		return;
	}

	if (state == GOOMBAFLY_STATE_WALKING && (GetTickCount64() - walking_start >= WALKING_TIME) && hasWing)
	{	
		if (GetTickCount64() - toward_mario_start >= TOWARD_MARIO_TIME)
		{
			toward_mario_start = -1;
			TowardMario(GOOMBA_WALKING_SPEED);
			SetState(GOOMBAFLY_STATE_SMALL_JUMP);
			start_small_jump = GetTickCount64();
		}
		walking_start = -1;
	}
	if (state == GOOMBAFLY_STATE_SMALL_JUMP && (GetTickCount64() - small_jump_start >= SMALL_JUMP_TIME) && !hasWing)
	{
		SetState(GOOMBAFLY_STATE_WALKING);
		walking_start = -1;
	}
	if (state == GOOMBAFLY_STATE_SMALL_JUMP && (GetTickCount64() - small_jump_start >= SMALL_JUMP_TIME) && hasWing)
	{	
		SetState(GOOMBAFLY_STATE_BIG_JUMP);
		toward_mario_start = GetTickCount64();
		small_jump_start = -1;
	}
	if (state == GOOMBAFLY_STATE_BIG_JUMP && (GetTickCount64() - big_jump_start >= BIG_JUMP_TIME) && hasWing)
	{
		SetState(GOOMBAFLY_STATE_WALKING);
		big_jump_start = -1;
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->ProcessCollision(this, dt, coObjects);
	CCollision::GetInstance()->ProcessOverlapNow(this, dt, coObjects);

}

void CGoombaFly::Render()
{
	if (GetIsPause() )
	{
		die_start = GetTickCount64();
		return;
	}
	int aniId = ID_ANI_GOOMBAFLY_WALKING;
	if (hasWing && state == GOOMBAFLY_STATE_WALKING) 
	{	
		if (!GetIsStop() && !GetIsDead())
		{
			CAnimations::GetInstance()->Get(ID_ANI_RIGHT_WING_WALKING)->Render(x - 6, y - 6);
			CAnimations::GetInstance()->Get(ID_ANI_LEFT_WING_WALKING)->Render(x + 6, y - 6);
		}
		else
		{
			CAnimations::GetInstance()->Get(ID_ANI_RIGHT_WING_WALKING)->Render(x - 6, y - 6, 1);
			CAnimations::GetInstance()->Get(ID_ANI_LEFT_WING_WALKING)->Render(x + 6, y - 6, 1);
		}
	}
	if (hasWing && state == GOOMBAFLY_STATE_SMALL_JUMP)
	{

		if (!GetIsStop() && !GetIsDead())
		{
			CAnimations::GetInstance()->Get(ID_ANI_RIGHT_WING_SMALL_JUMP)->Render(x - 6, y - 9);
			CAnimations::GetInstance()->Get(ID_ANI_LEFT_WING_SMALL_JUMP)->Render(x + 6, y - 9);
		}
		else
		{
			CAnimations::GetInstance()->Get(ID_ANI_RIGHT_WING_SMALL_JUMP)->Render(x - 6, y - 9, 1);
			CAnimations::GetInstance()->Get(ID_ANI_LEFT_WING_SMALL_JUMP)->Render(x + 6, y - 9, 1);
		}
	}
	if (hasWing && state == GOOMBAFLY_STATE_BIG_JUMP)
	{
		if (!GetIsStop() && !GetIsDead())
		{
			CAnimations::GetInstance()->Get(ID_ANI_RIGHT_WING_BIG_JUMP)->Render(x - 6, y - 9);
			CAnimations::GetInstance()->Get(ID_ANI_LEFT_WING_BIG_JUMP)->Render(x + 6, y - 9);
		}
		else
		{
			CAnimations::GetInstance()->Get(ID_ANI_RIGHT_WING_BIG_JUMP)->Render(x - 6, y - 9, 1);
			CAnimations::GetInstance()->Get(ID_ANI_LEFT_WING_BIG_JUMP)->Render(x + 6, y - 9, 1);
		}		
	}
	if (state == GOOMBAFLY_STATE_DIE)
	{
		aniId = ID_ANI_GOOMBAFLY_DIE;
	}
	if (state == GOOMBAFLY_STATE_DIE_UP && dir > 0)
	{
		aniId = ID_ANI_GOOMBAFLY_DIE_UP_LEFT;
	}
	if (state == GOOMBAFLY_STATE_DIE_UP && dir < 0)
	{
		aniId = ID_ANI_GOOMBAFLY_DIE_UP_RIGHT;
	}
	if(!GetIsStop() && !GetIsDead()) CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	else CAnimations::GetInstance()->Get(aniId)->Render(x, y, 1);

	RenderBoundingBox();
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
		if (state == GOOMBAFLY_STATE_SMALL_JUMP && GetTickCount64() - start_small_jump >= 200 ) 
		{	
			start_small_jump = -1;
			if (e->ny == -1 && hasWing) {
				vy = -GOOMBA_SMALL_JUMP_SPEED;
			}
		}
		else vy = 0;
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
	//vx = -GOOMBA_WALKING_SPEED;
	TowardMario(GOOMBA_WALKING_SPEED);
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
		ay = GOOMBA_GRAVITY;
		vy = 0;
		walking_start = GetTickCount64();
		
		//TowardMario();
		break;
	case GOOMBAFLY_STATE_SMALL_JUMP:
		ay = GOOMBA_JUMP_GRAVITY;
		small_jump_start = GetTickCount64();
		break;
	case GOOMBAFLY_STATE_BIG_JUMP:
		big_jump_start = GetTickCount64();
		vy = -GOOMBA_FLYING_SPEED;
		break;
	case GOOMBAFLY_STATE_DIE_UP:
		frontRender = true;
		if ((this->state == GOOMBAFLY_STATE_DIE_UP) && (GetTickCount64() - die_start <= GOOMBAFLY_DIE_TIMEOUT))
			break;
		die_start = GetTickCount64();
		ay = GOOMBA_GRAVITY;
		if (tailhit) vy = -GOOMBA_TAILHIT_SPEED_Y;
		else vy = -GOOMBA_FLYING_SPEED;
		killOffCam = true;
		break;
	}
	CGameObject::SetState(state);
}

void CGoombaFly::Stomped()
{
	if (hasWing) {
		SetState(GOOMBAFLY_STATE_WALKING);
		hasWing = false;
	}
	else SetState(GOOMBAFLY_STATE_DIE);
}

void CGoombaFly::ShellHit(int shellX)
{	

	if (shellX == -1)
	{
		vx = GOOMBA_FLYING_SPEED_X;
		dir = -1;
	}
	else if (shellX == 1)
	{
		dir = 1;
		vx = -GOOMBA_FLYING_SPEED_X;
	}
	else if (shellX < x)
	{
		dir = -1;
		vx = GOOMBA_FLYING_SPEED_X;
	}
	else if (shellX > x)
	{
		dir = 1;
		vx = -GOOMBA_FLYING_SPEED_X;
	}
	else
	{
		dir = 0;
		vx = 0;
	}
	//die_start = GetTickCount64();
	SetState(GOOMBAFLY_STATE_DIE_UP);
}

void CGoombaFly::TailHit(float x)
{	
	if (x < this->x)
	{
		dir = -1;
		vx = GOOMBA_FLYING_SPEED_X;
	}
	else
	{
		dir = 1;
		vx = -GOOMBA_FLYING_SPEED_X;
	}
	tailhit = true;
	//die_start = GetTickCount64();
	SetState(GOOMBAFLY_STATE_DIE_UP);
}

void CGoombaFly::TowardMario(float speed, float awaymario)
{
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	float mario_x = 0, mario_y = 0;
	CMario* mario = (CMario*)scene->GetPlayer();
	if (mario != NULL) mario->GetPosition(mario_x, mario_y);
	if (mario_x <= x) {
		vx = -speed * awaymario;
	}
	else {
		vx = speed * awaymario;
	}
}
