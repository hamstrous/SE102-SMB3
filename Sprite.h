#pragma once

#include "Texture.h"

class CSprite
{
protected:
	int id;				// Sprite ID in the sprite database

	int left;
	int top;
	int right;
	int bottom;

	float offsetX = 0;
	float offsetY = 0;

	bool onScreen = false;

	LPTEXTURE texture;
	D3DX10_SPRITE sprite;
	D3DXMATRIX matScaling;
public:
	CSprite(int id, int left, int top, int right, int bottom, LPTEXTURE tex, bool onScreen = false, float offsetX = 0, float offsexY = 0);

	virtual void Draw(float x, float y);
	virtual void DrawOnCamera(float x, float y);
	virtual void DrawOnScreen(float x, float y);
};

typedef CSprite* LPSPRITE;