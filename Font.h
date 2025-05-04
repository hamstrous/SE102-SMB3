#pragma once
#include <string>
#include <unordered_map>
using namespace std;

#define FONT_SIZE 8

class CFont
{
	static CFont* __instance;
	static const unordered_map<char, int> charSpriteIdMap;
	static const unordered_map<int, int> intSpriteIdMap;
public:
	CFont() {};
	void FontToSprite(int x, int y, string str);
	void FontToSprite(int x, int y, int number);
	void FontToSprite(int x, int y, int number, int length);
	static CFont* GetInstance();
};

