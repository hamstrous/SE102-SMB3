#include "CloudPlatform.h"

void CCloudPlatform::Render()
{
	CSprites::GetInstance()->Get(aniId)->Draw(x, y);
}
