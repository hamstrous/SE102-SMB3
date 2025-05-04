#include "Font.h"
#include "Sprite.h"
#include "Sprites.h"
#include "debug.h"
#include "Utils.h"
#include <unordered_map>

const std::unordered_map<char, int> CFont::charSpriteIdMap = {
    {'0', 8000},
    {'1', 8001},
    {'2', 8002},
    {'3', 8003},
    {'4', 8004},
    {'5', 8005},
    {'6', 8006},
    {'7', 8007},
    {'8', 8008},
    {'9', 8009},
    {'A', 8010},
    {'B', 8011},
    {'C', 8012},
    {'D', 8013},
    {'E', 8014},
    {'F', 8015},
    {'G', 8016},
    {'H', 8017},
    {'I', 8018},
    {'J', 8019},
    {'K', 8020},
    {'L', 8021},
    {'M', 8022},
    {'N', 8023},
    {'O', 8024},
    {'P', 8025},
    {'Q', 8026},
    {'R', 8027},
    {'S', 8028},
    {'T', 8029},
    {'U', 8030},
    {'V', 8031},
    {'W', 8032},
    {'X', 8033},
    {'Y', 8034},
    {'Z', 8035}
};

const std::unordered_map<int, int> CFont::intSpriteIdMap = {
    {0, 8000},
    {1, 8001},
    {2, 8002},
    {3, 8003},
    {4, 8004},
    {5, 8005},
    {6, 8006},
    {7, 8007},
    {8, 8008},
    {9, 8009}
};

void CFont::FontToSprite(int x, int y, string str)
{
    ToUpperCase(str);

    int strLength = str.size();
    for (int i = 0; i < strLength; i++)
    {
        char c = str[i];
        auto it = charSpriteIdMap.find(c);
        if (it != charSpriteIdMap.end())
        {
            int spriteId = it->second;
            // Adjust the x position for right anchor: start from the rightmost edge
            int posX = x - (strLength - i - 1) * FONT_SIZE;
            CSprites::GetInstance()->Get(spriteId)->Draw(posX, y);
        }
        else
        {
            // Handle unknown character
            DebugOutTitle(L"[ERROR] CFont::FontToSprite: Unknown character %c\n", c);
        }
    }
}


void CFont::FontToSprite(int x, int y, int number)
{
    if (number < 0) number = 0;
    std::string str = std::to_string(number);
    FontToSprite(x, y, str);
}

void CFont::FontToSprite(int x, int y, int number, int length)
{
    if (number < 0) number = 0;
	std::string str = std::to_string(number);
	int strLength = length - str.size();
    if(strLength < 0) strLength = 0; // Prevent negative length
	while(strLength--) str = '0' + str;
	FontToSprite(x, y, str);
}

CFont* CFont::GetInstance()
{
    if (__instance == NULL) __instance = new CFont();
    return __instance;
}

CFont* CFont::__instance = NULL;
