#include "Headbush.h"

void CHeadbush::Render()
{
	float xx = x;
	CSprites* s = CSprites::GetInstance();

	s->Get(this->spriteIdBegin)->Draw(x, y);
	xx += this->cellWidth;
	s->Get(this->spriteIdEnd)->Draw(xx, y);
	//RenderBoundingBox();
}

void CHeadbush::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	float cellWidth_div_2 = this->cellWidth / 2;
	l = x - cellWidth_div_2;
	t = y - this->cellHeight / 2;
	r = l + this->cellWidth;
	b = t + this->cellHeight;
}

void CHeadbush::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;
	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);
	float l, t, r, b;
	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;
	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);
	float xx = x - this->cellWidth / 2 + rect.right / 2;
	CGame::GetInstance()->Draw(xx - cx, y - cy, bbox, nullptr, BBOX_ALPHA, rect.right - 1, rect.bottom - 1);
}

int CHeadbush::IsDirectionColliable(float nx, float ny)
{
	return 0;
}
