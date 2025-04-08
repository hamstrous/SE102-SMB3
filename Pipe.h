#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "Sprites.h"

class CPipe : public CGameObject
{
protected:
	int height;
	float cellWidth;
	float cellHeight;
	int spriteIdBegin, spriteIdEnd;

public:
	CPipe(float x, float y,
		float cell_width, float cell_height, int height,
		int sprite_id_begin, int sprite_id_end) :CGameObject(x, y)
	{
		this->height = height;
		this->cellWidth = cell_width;
		this->cellHeight = cell_height;
		this->spriteIdBegin = sprite_id_begin;
		this->spriteIdEnd = sprite_id_end;
	}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();
	int IsDirectionColliable(bool a);

};

typedef CPipe* LPPipe;