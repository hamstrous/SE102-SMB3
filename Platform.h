#pragma once

#include "GameObject.h"
#include "GenericPlatform.h"

// 
// The most popular type of object in Mario! 
// 
class CPlatform : public CGenericPlatform
{
protected:
	int length;				// Unit: cell 
	float cellWidth;
	float cellHeight;
	int spriteIdBegin, spriteIdMiddle, spriteIdEnd;
	int blocking;

public:
	CPlatform(float x, float y,
		float cell_width, float cell_height, int length,
		int sprite_id_begin, int sprite_id_middle, int sprite_id_end,
		int blocking) :CGenericPlatform(x, y)
	{
		this->length = length;
		this->cellWidth = cell_width;
		this->cellHeight = cell_height;
		this->spriteIdBegin = sprite_id_begin;
		this->spriteIdMiddle = sprite_id_middle;
		this->spriteIdEnd = sprite_id_end;
		this->blocking = blocking;
	}

	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();

	int IsDirectionColliable(float nx, float ny);
};

typedef CPlatform* LPPLATFORM;