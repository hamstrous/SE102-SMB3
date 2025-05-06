#include "ColorBlock.h"
#include "Textures.h"
#include "Game.h"

CColorBlock::CColorBlock(float x, float y, int width, int height, int color = 0) : CGenericPlatform(x,y)
{
	this->width = width;
	this->height = height;
	this->color = color;
}

void CColorBlock::Render()
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			CSprites::GetInstance()->Get(XYtoSpriteID(i, j, color))->Draw(x + j * TILE_SIZE, y + i * TILE_SIZE);
		}
	}

	// Draw shadow
	for (int i = 0; i < height + 1; i++)
	{
		CSprites::GetInstance()->Get(shadow[0][CheckCondition(i,height + 1)])->Draw(x + width * TILE_SIZE, y + i * TILE_SIZE);
	}

	for (int i = 0; i < width + 1; i++)
	{
		CSprites::GetInstance()->Get(shadow[1][CheckCondition(i, width + 1)])->Draw(x + i * TILE_SIZE, y + height * TILE_SIZE);
	}
}

void CColorBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - TILE_SIZE / 2;
	t = y - TILE_SIZE / 2;
	r = l + this->width * TILE_SIZE;
	b = t + TILE_SIZE;
}

