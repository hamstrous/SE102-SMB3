#include "Floor.h"

CFloor::CFloor(float x, float y, int width, int height, int type) : CBaseBrick(x, y)
{
	this->width = width;
	this->height = height;
	this->type = type;
}

void CFloor::Render()
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			CSprites::GetInstance()->Get(XYtoSpriteID(i, j, type))->Draw(x + j * TILE_SIZE, y + i * TILE_SIZE);
		}
	}
}

void CFloor::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - TILE_SIZE / 2.f;
	t = y - TILE_SIZE / 2.f;
	r = l + this->width * TILE_SIZE;
	b = t + this->height * TILE_SIZE;
}

void CFloor::RenderBoundingBox()
{
}
