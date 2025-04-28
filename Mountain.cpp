#include "Mountain.h"

void CMountain::Render()
{

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			CSprites::GetInstance()->Get(XYtoSpriteID(i, j, type, color))->Draw(x + j * TILE_SIZE, y + i * TILE_SIZE);
		}
	}
}

CMountain::CMountain(float x, float y, int width, int height, int type, int color)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->type = type;
	this->color = color;

	if (type == 1) {
		this->width = 2;
		this->height = 1;
	}
}
