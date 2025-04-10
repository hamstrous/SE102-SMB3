#include "Plant.h"
#include "Game.h"
#include "Mario.h"
#include "Pipe.h"
#include "PlayScene.h"
#include "Fireball.h"
void CPlant::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - PRIRANHA_BBOX_WIDTH / 2;
	top = y - PRIRANHA_BBOX_HEIGHT / 2;
	right = left + PRIRANHA_BBOX_WIDTH;
	bottom = top + PRIRANHA_BBOX_HEIGHT + STEM_BBOX_HEIGHT;
}

void CPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	LPGAME game = CGame::GetInstance();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	float marioX, marioY;
	mario->GetPosition(marioX, marioY);
	
	if ((state == STATE_PRIRANHA_UP) && startY - y > PIRANHA_BBOX )
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
	if ((state == STATE_PRIRANHA_DOWN) && startY < y )
	{	
			y = startY;
			if (marioX < 300 || marioX > 400)
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
						y  >= cy && y  <= cy + camHeight))
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
	
	CAnimations::GetInstance()->Get(ID_ANI_STEM)->Render(x, y + (PRIRANHA_BBOX_HEIGHT / 2) + STEM_BBOX_HEIGHT);
	CAnimations::GetInstance()->Get(GetaniID())->Render(x, y);
	RenderBoundingBox();
}

void CPlant::ShootFireball()
{
	LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	CFireball* fireball = new CFireball(x, y);
	//fireball->SetState(FIREBALL_STATE_PIRANHA);
	float mario_x, mario_y, a;
	scene->GetPlayer()->GetPosition(mario_x, mario_y);
	if (mario_x < x)
	{
		if (mario_y < y && mario_x > 264)
		{
			fireball->SetSpeed(-FIREBALL_SPEED, -FIREBALL_SPEED * 0.8f );
		}
		else if (mario_y < y && mario_x < 264)
		{
			fireball->SetSpeed(-FIREBALL_SPEED, -FIREBALL_SPEED * 0.2f);
		}
		else if (mario_y > y && mario_x < 264)
		{
			fireball->SetSpeed(-FIREBALL_SPEED, FIREBALL_SPEED * 0.2f);
		}
		else if (mario_y > y && mario_x > 264)
		{
			fireball->SetSpeed(-FIREBALL_SPEED, FIREBALL_SPEED * 0.5f );
		}
	}
	else
	{
		if (mario_y < y && mario_x < 416)
		{
			fireball->SetSpeed(FIREBALL_SPEED, -FIREBALL_SPEED * 0.8f);
		}
		else if (mario_y < y && mario_x > 416)
		{
			fireball->SetSpeed(FIREBALL_SPEED, -FIREBALL_SPEED * 0.2f);
		}
		else if (mario_y > y && mario_x > 416)
		{
			fireball->SetSpeed(FIREBALL_SPEED, FIREBALL_SPEED * 0.2f);
		}
		else if (mario_y > y && mario_x < 416)
		{
			fireball->SetSpeed(FIREBALL_SPEED, FIREBALL_SPEED * 0.5f);
		}
	}
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
			aniId = ID_ANI_PRIRANHA_RED_UP_LEFT;
		else
			aniId = ID_ANI_PRIRANHA_RED_DOWN_LEFT;
	}
	else if (marioX > x)
	{
		if (marioY < y)
			aniId = ID_ANI_PRIRANHA_RED_UP_RIGHT;
		else
			aniId = ID_ANI_PRIRANHA_RED_DOWN_RIGHT;
	}
	if (state == STATE_PRIRANHA_STOP)
	{ 
		if (marioX < x)
		{
			if (marioY < y)
				aniId = ID_ANI_PRIRANHA_RED_UP_LEFT_STAY;
			else
				aniId = ID_ANI_PRIRANHA_RED_DOWN_LEFT_STAY;
		}
		else if (marioX > x)
		{
			if (marioY < y)
				aniId = ID_ANI_PRIRANHA_RED_UP_RIGHT_STAY;
			else
				aniId = ID_ANI_PRIRANHA_RED_DOWN_RIGHT_STAY;
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


CPlant::CPlant(float x, float y)
{
	this->x = x;
	this->y = y;
	this->isDeleted = false;
	startY = y;
	isFired = false;
	this->up_start = GetTickCount64();
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
		vy = -PIRANHA_SPEED;
		up_start = 0;
		break;
	case STATE_PRIRANHA_STOP:
		if (up_start == 0)
			up_start = GetTickCount64();
		vy = 0;
		break;
	case STATE_PRIRANHA_DOWN:
		vy = PIRANHA_SPEED;
		if (marioX > 300 || marioX < 400)
			up_start = GetTickCount64();
		else up_start = 0;
		break;
	}
	CGameObject::SetState(state);
}
