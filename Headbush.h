#pragma once
#include "GameObject.h"
#include "Sprite.h"
#include "Sprites.h"

#include "Textures.h"
#include "Game.h"
class CHeadbush : public CGameObject
{
protected:			// Unit: cell 
	float cellWidth;
	float cellHeight;
	int spriteIdBegin, spriteIdEnd;

public:
	CHeadbush(float x, float y,
		float cell_width, float cell_height,
		int sprite_id_begin, int sprite_id_end) :CGameObject(x, y)
	{
		this->cellWidth = cell_width;
		this->cellHeight = cell_height;
		this->spriteIdBegin = sprite_id_begin;
		this->spriteIdEnd = sprite_id_end;
	}

	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();

	int IsDirectionColliable(float nx, float ny);
};

