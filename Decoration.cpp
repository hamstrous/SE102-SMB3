#include "Decoration.h"

void CDecoration::Render()
{
	CSprites::GetInstance()->Get(colorSpriteId[type])->Draw(x, y);

}
