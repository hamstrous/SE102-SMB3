#include "CloudEllipse.h"

void CCloudEllipse::Render()
{
	if(type == TYPE_SMALL) CSprites::GetInstance()->Get(ID_ANI_SMALL_CLOUD)->Draw(x, y);
	else {
		CSprites::GetInstance()->Get(ID_ANI_LEFT_BIG_CLOUD)->Draw(x, y);
		CSprites::GetInstance()->Get(ID_ANI_RIGHT_BIG_CLOUD)->Draw(x + 16, y);
	}
}
