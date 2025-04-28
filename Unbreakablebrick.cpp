#include "Unbreakablebrick.h"

void CUnbreakablebrick::Render()
{
	CSprites::GetInstance()->Get(brickSpriteId[type])->Draw(x, y);
}
