#include "InvisibleWall.h"
#include "debug.h"

CInvisibleWall::CInvisibleWall(float x, float y, float width, float height) : CBaseBrick(x, y)
{
	this->width = width;
	this->height = height;

	// Set the state to static by default
	SetState(INVISIBLE_WALL_STATE_STATIC);
}

CInvisibleWall::CInvisibleWall(float x, float y, float width, float height, float endX, int state) : CBaseBrick(x, y)
{
	this->width = width;
	this->height = height;
	this->endX = endX;

	SetState(state); 
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
	RenderBoundingBox();
}
