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

public:
	~CSprites() {
		Clear();
	};
	void Add(int id, int left, int top, int right, int bottom, LPTEXTURE tex, bool onScreen = false, float offsetX = 0, float offsetY = 0);
	LPSPRITE Get(int id);
	void Clear();

	// New function to handle sprite overlap when Mario is hiding

	static CSprites* GetInstance();
};

