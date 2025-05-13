#include "Plant.h"
#include "Game.h"
#include "Mario.h"
#include "Pipe.h"
#include "PlayScene.h"
#include "Fireball.h"
#include <map>


void CPlant::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{	
	left = x - PRIRANHA_BBOX_WIDTH / 2;
	top = y - PRIRANHA_BBOX_HEIGHT / 2;
	right = left + PRIRANHA_BBOX_WIDTH;
	bottom = top + PRIRANHA_BBOX_HEIGHT + size;
}

void CPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	float marioX, marioY;
	mario->GetPosition(marioX, marioY);
	
	if (type == 0)
	{
		if (y < TAILHIT_DISTANCE1) tailhit = true;
		else tailhit = false;
	}
	else
	{
		if (y < TAILHIT_DISTANCE2) tailhit = true;
		else tailhit = false;
	}

	if ((state == STATE_PRIRANHA_UP) && startY - y > PIRANHA_BBOX - size )
	{
		SetState(STATE_PRIRANHA_STOP);
	}

	if ((state == STATE_PRIRANHA_STOP) && (GetTickCount64() - up_start > PRIRANHA_STOP_TIMEOUT))
	{
		if (y < startY)
			SetState(STATE_PRIRANHA_DOWN);
		else
			SetState(STATE_PRIRANHA_UP);
	}
	if ((state == STATE_PRIRANHA_DOWN) && startY < y - type*-20)
	{	
		y = startY;
		if (marioX < x-30 || marioX > x+30)
		{
			SetState(STATE_PRIRANHA_STOP);
		}

	}
	//If fireball in screen?
	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);
	float camWidth = CGame::GetInstance()->GetScreenWidth();
	float camHeight = CGame::GetInstance()->GetScreenHeight();
	if (state == STATE_PRIRANHA_STOP && y < startY && (GetTickCount64() - up_start > PRIRANHA_STOP_TIMEOUT / 2) 
		&& isFired && ( x  >= cx && x  <= cx + camWidth &&
						y  >= cy && y  <= cy + camHeight) && type == 0)
	{
		ShootFireball();
	}


	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CPlant::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);

	float yy = y - PIRANHA_BBOX / 2 + PRIRANHA_BBOX_HEIGHT / 2 + rect.right / 2;

	CGame::GetInstance()->Draw(x - cx, yy - cy, bbox, nullptr, BBOX_ALPHA, rect.right - 1, rect.bottom - 1);
}
void CPlant::Render()
{
	if (GetIsPause()) return;
	if (!GetIsStop()) CAnimations::GetInstance()->Get(GetaniID())->Render(x, y);
	else CAnimations::GetInstance()->Get(GetaniID())->Render(x, y, 1);
	//RenderBoundingBox();
}

void CPlant::ShootFireball()
{
	const float FIREBALL_SPEED_X = 0.06f; 
	const float FIREBALL_ANGLES[] = {
		225.0f,  // Above near
		202.5f, // Above far
		162.5f, // Under far
		135.0f // Under near
	};

	LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	CFireball* fireball = new CFireball(x, y + size);
	float mario_x, mario_y;
	scene->GetPlayer()->GetPosition(mario_x, mario_y);

	// Distance mario-plant
	float dx = mario_x - x;
	float dy = mario_y - y;

	bool toLeft = dx < 0; // Mario on the left
	bool isAbove = dy < 0; // Mario on above

	// Get angle
	int angleIndex;
	if (isAbove)
	{
		angleIndex = abs(dx) <= 80 ? 0 : 1; // above near : far
	}
	else
	{
		angleIndex = abs(dx) <= 80 ? 3 : 2; // under near : far
	}

	// get angle
	float angle = FIREBALL_ANGLES[angleIndex];

	// from angle to radians
	float radians = angle * (3.14159265f / 180.0f);

	// cal fbvx fbvy
	float fbvx = FIREBALL_SPEED_X * cos(radians);
	float fbvy = FIREBALL_SPEED_X * sin(radians);

	// if mario left, swap
	if (!toLeft)
	{
		fbvx = -fbvx;
	}

	// set speed for fireball
	fireball->SetSpeed(fbvx, fbvy);
	scene->AddObject(fireball);
	isFired = false;
}

int CPlant::GetaniID()
{	
	LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	float marioX, marioY;
	scene->GetPlayer()->GetPosition(marioX, marioY);
	int aniId = ID_ANI_PRIRANHA_RED_DOWN_LEFT;
	if (marioX < x)
	{
		if (marioY < y)
			aniId = ID_ANI_PRIRANHA_RED_UP_LEFT + color + (type*9);
		else
			aniId = ID_ANI_PRIRANHA_RED_DOWN_LEFT + color + (type*10);
	}
	else if (marioX > x)
	{
		if (marioY < y)
			aniId = ID_ANI_PRIRANHA_RED_UP_RIGHT + color + (type*7);
		else
			aniId = ID_ANI_PRIRANHA_RED_DOWN_RIGHT + color + (type*8);
	}
	if (state == STATE_PRIRANHA_STOP)
	{ 
		if (marioX < x)
		{
			if (marioY < y)
				aniId = ID_ANI_PRIRANHA_RED_UP_LEFT_STAY + color + (type * 5);
			else
				aniId = ID_ANI_PRIRANHA_RED_DOWN_LEFT_STAY + color + (type * 6);
		}
		else if (marioX > x)
		{
			if (marioY < y)
				aniId = ID_ANI_PRIRANHA_RED_UP_RIGHT_STAY + color + (type * 3);
			else
				aniId = ID_ANI_PRIRANHA_RED_DOWN_RIGHT_STAY + color + (type * 4);
		}
	}
	return aniId;
}



void CPlant::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CPlant::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;

}


CPlant::CPlant(float x, float y, int color, int type, int size) :CCharacter(x, y)
{
	this->x = x;
	this->y = y;
	this->isDeleted = false;
	startY = y;
	isFired = false;
	this->color = color;
	this->type = type;
	this->up_start = GetTickCount64();
	this->size = size;
	SetState(STATE_PRIRANHA_UP);
}



void CPlant::SetState(int state)
{	
	LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	float marioX, marioY;
	scene->GetPlayer()->GetPosition(marioX, marioY);
	switch (state)
	{
	case STATE_PRIRANHA_UP:
		isFired = true;
		vy = -(PIRANHA_SPEED + type*0.02);
		up_start = 0;
		break;
	case STATE_PRIRANHA_STOP:
		if (up_start == 0)
			up_start = GetTickCount64();
		vy = 0;
		break;
	case STATE_PRIRANHA_DOWN:
		vy = (PIRANHA_SPEED + type * 0.02);
		if (marioX > x-50 || marioX < x+50)
			up_start = GetTickCount64();
		else up_start = 0;
		break;
	case STATE_PRIRANHA_SHELL_HIT:
		isDeleted = true;
		CSmoke* smoke = new CSmoke(x, y);
		((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->AddObject(smoke);
	}
	CGameObject::SetState(state);
}

void CPlant::Stomped()
{
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	mario->Attacked();
}

void CPlant::ShellHit(int shellX)
{
	SetState(STATE_PRIRANHA_SHELL_HIT);
}

void CPlant::TailHit(float x)
{
	SetState(STATE_PRIRANHA_SHELL_HIT);
	DebugOut(L"PLANT TAIL HIT\n");
}


