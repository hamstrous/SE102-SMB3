#pragma once
#include <Windows.h>
#include <unordered_map>
#include <vector>

#include <d3d10.h>
#include <d3dx10.h>

#include "Texture.h"
#include "Sprite.h"

using namespace std;

/*
	Manage sprite database
*/
class CSprites
{
	static CSprites* __instance;

	unordered_map<int, LPSPRITE> sprites;

	// Helper function to get pixel data from texture
	vector<DWORD> GetTexturePixels(LPTEXTURE tex, int left, int top, int right, int bottom);
	// Helper function to check if pixel is transparent
	bool IsPixelTransparent(DWORD color);

public:
	void Add(int id, int left, int top, int right, int bottom, LPTEXTURE tex, bool onScreen = false, float offsetX = 0, float offsetY = 0);
	LPSPRITE Get(int id);
	void Clear();

	// New function to handle sprite overlap when Mario is hiding
	LPSPRITE CreateOverlapSprite(LPSPRITE background, LPSPRITE mario, float bgX, float bgY, float marioX, float marioY);

	static CSprites* GetInstance();
};

