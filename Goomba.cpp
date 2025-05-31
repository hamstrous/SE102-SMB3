#include "Goomba.h"
#include "Smoke.h"
#include "PlayScene.h"
#include "Mario.h"

CGoomba::CGoomba(float x, float y):CCharacter(x, y)
{
	this->ax = 0;
	this->ay = GOOMBA_GRAVITY;
	die_start = -1;
	tailhit = false;
	SetState(GOOMBA_STATE_WALKING);
	TowardMario(GOOMBA_WALKING_SPEED);
}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (state == GOOMBA_STATE_DIE || state == GOOMBA_STATE_DIE_UP)
	{
		left = top = right = bottom = 0;
	}
	else
	{ 
		left = x - GOOMBA_BBOX_WIDTH/2;
		top = y - GOOMBA_BBOX_HEIGHT/2;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT;
	}
	
}

void CGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return; 
	if (dynamic_cast<CGoomba*>(e->obj)) return; 

	if (e->ny != 0 )
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

float pvx = 0;
void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	pvx = vx;
	vy += ay * dt;
	vx += ax * dt;

	vy = min(vy, 0.23f);

	if ( (state==GOOMBA_STATE_DIE) && (GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT) )
	{
		isDeleted = true;
		return;
	}
	else if ( (state == GOOMBA_STATE_DIE_UP) && (GetTickCount64() - die_start > GOOMBA_DIE_UP_TIMEOUT) )
	{
		isDeleted = true;
		return;
	}
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->ProcessCollision(this, dt, coObjects);
	CCollision::GetInstance()->ProcessOverlapNow(this, dt, coObjects);
}


void CGoomba::Render()
{	
	if (GetIsPause()) return;
	int aniId = ID_ANI_GOOMBA_WALKING;
	if (state == GOOMBA_STATE_DIE) 
	{
		aniId = ID_ANI_GOOMBA_DIE;
	}
	if (state == GOOMBA_STATE_DIE_UP && dir>0)
	{
		aniId = ID_ANI_GOOMBA_DIE + 1;
	}
	if (state == GOOMBA_STATE_DIE_UP && dir < 0)
	{
		aniId = ID_ANI_GOOMBA_DIE + 2;
	}
	if (!GetIsStop() && !GetIsDead()) CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	else CAnimations::GetInstance()->Get(aniId)->Render(x, y, 1);
	//RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	switch (state)
	{
		case GOOMBA_STATE_DIE:
			if ((this->state == GOOMBA_STATE_DIE) && (GetTickCount64() - die_start <= GOOMBA_DIE_TIMEOUT))
				break;
			// Set the die animation
			die_start = GetTickCount64();
			y += (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE)/2;
			vx = 0;
			vy = 0;
			ay = 0; 
			break;
		case GOOMBA_STATE_WALKING: 
			vx = -GOOMBA_WALKING_SPEED;
			break;
		case GOOMBA_STATE_DIE_UP:
			frontRender = true;
			ay = GOOMBA_GRAVITY;
			die_start = GetTickCount64();
			if (tailhit) vy = -GOOMBA_TAILHIT_SPEED_Y;
			else vy = -GOOMBA_FLYING_SPEED;
			killOffCam = true;
			break;
	}
	CGameObject::SetState(state);
}

void CGoomba::Stomped()
{	
	SetState(GOOMBA_STATE_DIE);
}

void CGoomba::ShellHit(int shellX)
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
	SetState(GOOMBA_STATE_DIE_UP);
}

void CGoomba::TailHit(float x)
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
	SetState(GOOMBA_STATE_DIE_UP);
}

void CGoomba::TowardMario(float speed, float awayMario)
{
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	float mario_x = 0, mario_y = 0;
	CMario* mario = (CMario*)scene->GetPlayer();
	if (mario != NULL) mario->GetPosition(mario_x, mario_y);
	if (mario_x <= x) {
		vx = -speed * awayMario;
	}
	else {
		vx = speed * awayMario;
	}
}

