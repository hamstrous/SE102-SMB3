#pragma once
#include "GameObject.h"
#define TILE_SIZE 16

class CBackgroundColor : public CGameObject
{
	int width;				// Unit: cell 
	int height;				// Unit: cell 
	int color; // 0: Blue, 1: Black
	int colorSpriteId[2] = { 170004, 170005 };
public:
	CBackgroundColor(float x, float y,int width, int height, int color) : CGameObject(x, y) {
		this->width = width;
		this->height = height;
		this->color = color;
	}
	void GetBoundingBox(float& l, float& t, float& r, float& b) {
		l = t = r = b = 0;
	}
	void Render();
};

