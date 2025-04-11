#include "Pipe.h"

#include "Sprite.h"
#include "Sprites.h"
#include "Game.h"
#include "Textures.h"

void CPipe::RenderBoundingBox()
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
	float yy = y - this->cellHeight / 2 + rect.bottom / 2;
	float xx = x + this->cellWidth / 2;
	CGame::GetInstance()->Draw(xx - cx, yy - cy, bbox, nullptr, BBOX_ALPHA, rect.right - 1, rect.bottom - 1);
}

void CPipe::Render()
{
	if (this->height <= 0) return;
	float yy = y;
	float xx = x;
	CSprites* s = CSprites::GetInstance();

	s->Get(this->spriteId_top_left)->Draw(x, y);
	xx += this->cellWidth;
	s->Get(this->spriteId_top_right)->Draw(xx, y);
	for (int i = 1; i < this->height; i++)
	{
		yy += this->cellHeight;
		s->Get(this->spriteId_bot_left)->Draw(x, yy);
		s->Get(this->spriteId_bot_right)->Draw(xx, yy);
	}

	//RenderBoundingBox();
}

void CPipe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - this->cellWidth / 2;
	t = y - this->cellHeight / 2;
	r = l + this->cellWidth * 2;
	b = t + this->cellHeight * this->height;
}