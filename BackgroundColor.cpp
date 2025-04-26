#include "BackgroundColor.h"

void CBackgroundColor::Render()
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			CSprites::GetInstance()->Get(colorSpriteId[color])->Draw(x + j * TILE_SIZE, y + i * TILE_SIZE);
		}
	}
}
