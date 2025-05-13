#include "Point.h"

#include "Textures.h"
#include "Game.h"

void CPoint::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_POINT);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	l -= 0.5f;
	t -= 0.5f;
	r += 0.5f;
	b += 0.5f;
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float cx, cy;
	CGame::GetInstance()->GetCurrentScene()->GetCamera()->GetPosition(cx, cy);

	CGame::GetInstance()->Draw(x - cx, y - cy, bbox, &rect, POINT_ALPHA);
};
