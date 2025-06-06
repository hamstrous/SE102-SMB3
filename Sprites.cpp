#include "Sprites.h"
#include "Game.h"
#include "debug.h"
#include "Textures.h"

CSprites* CSprites::__instance = NULL;

CSprites* CSprites::GetInstance()
{
	if (__instance == NULL) __instance = new CSprites();
	return __instance;
}

void CSprites::Add(int id, int left, int top, int right, int bottom, LPTEXTURE tex, bool onScreen, float offsetX, float offsetY)
{
	LPSPRITE s = new CSprite(id, left, top, right, bottom, tex, onScreen, offsetX, offsetY);
	if (sprites.find(id) != sprites.end())
	{
		DebugOut(L"[ERROR] Sprite with ID %d already exists!\n", id);
		delete s; // Avoid memory leak
		return;
	}
	sprites[id] = s;
}

LPSPRITE CSprites::Get(int id)
{
	return sprites[id];
}

/*
	Clear all loaded sprites
*/
void CSprites::Clear()
{
	for (auto x : sprites)
	{
		LPSPRITE s = x.second;
		delete s;
	}

	sprites.clear();
}