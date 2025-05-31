#include "InvisibleWall.h"
#include "debug.h"
#include "PlayScene.h"
#include "Mario.h"
#include "Game.h"

CInvisibleWall::CInvisibleWall(float x, float y, float width, float height) : CGameObject(x, y)
{
	this->width = width;
	this->height = height;

	// Set the state to static by default
	SetState(INVISIBLE_WALL_STATE_STATIC);
}

CInvisibleWall::CInvisibleWall(float x, float y, float width, float height, float endX, int state) : CGameObject(x, y)
{
	this->width = width;
	this->height = height;
	this->endX = endX;

	SetState(state); 
}

void CInvisibleWall::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	{
		l = x - width / 2;
		t = y - height / 2;
		r = x + width;
		b = y + height;
		CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		CMario* mario = dynamic_cast<CMario*>(scene->GetPlayer());
		if (mario->GetState() == MARIO_STATE_WIN) {
			l = t = r = b = 0;
		}
	};
}

void CInvisibleWall::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Update the position of the invisible wall
	if (state == INVISIBLE_WALL_STATE_MOVING && x < endX)
	{
		x += vx * dt;
	}
}

void CInvisibleWall::Render()
{
	//RenderBoundingBox();
}
