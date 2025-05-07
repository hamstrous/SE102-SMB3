#include "UnbreakableBrick.h"


void CUnbreakableBrick::Render()
{
	CSprites::GetInstance()->Get(brickSpriteId[type])->Draw(x, y);
}
