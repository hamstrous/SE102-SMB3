#include "InvisibleWall.h"
#include "debug.h"

CInvisibleWall::CInvisibleWall(float x, float y, float width, float height) : CBaseBrick(x, y)
{
	this->width = width;
	this->height = height;

	vx = INVISIBLE_WALL_SPEED; // Set the speed of the wall

	// Set the state to static by default
	SetState(INVISIBLE_WALL_STATE_STATIC);
	//state = INVISIBLE_WALL_STATE_MOVING;
}

void CInvisibleWall::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Update the position of the invisible wall
	if (state == INVISIBLE_WALL_STATE_MOVING)
	{
		x += vx * dt;
	}
}

void CInvisibleWall::Render()
{
	RenderBoundingBox();
}
