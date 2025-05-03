#pragma once
#include "GameObject.h"
#include "Sprite.h"
#include <string>

using namespace std;

class CFont : public CGameObject
{
	static const unordered_map<char, int> spriteIdMap;
public:
	CFont();
	void FontToSprite(string str);
	void FontToSprite(int number);
};

