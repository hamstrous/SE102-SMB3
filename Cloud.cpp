#include "Cloud.h"
#include "debug.h"

void CCloud::Render()
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < width + 2; j++)
		{
			CSprites::GetInstance()->Get(XYtoSpriteID(i, j))->Draw(x + j * TILE_SIZE, y + i * TILE_SIZE);
		}
	}
}
