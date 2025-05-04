#include "HUD.h"
#include "Sprites.h"
#include "Game.h"
#include "debug.h"
#include "Font.h"

#define LIFE_X_OFFSET		-75
#define LIFE_Y_OFFSET		4

#define SCORE_X_OFFSET		-12
#define SCORE_Y_OFFSET		4

#define COIN_X_OFFSET		10
#define COIN_Y_OFFSET		10

#define WORLD_X_OFFSET		10
#define WORLD_Y_OFFSET		10

#define COUNTDOWN_X_OFFSET		28
#define COUNTDOWN_Y_OFFSET		4

#define COIN_X_OFFSET		28
#define COIN_Y_OFFSET		-4

CHUD::CHUD()
{
	gameData = CGameData::GetInstance();
	endTimer = new CTimer(-1);
}

void CHUD::Render()
{
	CSprites* sprites = CSprites::GetInstance();
	CGame* game = CGame::GetInstance();
	CFont* font = CFont::GetInstance();
	DebugOutTitle(L"HUD Render");

	float hx = game->GetBackBufferWidth() / 2;
	float hy = game->GetBackBufferHeight() - HUD_SIZE_Y/2;
	sprites->Get(ID_SPRITE_HUD)->Draw(hx, hy);

	int life = gameData->life;
	font->FontToSprite(hx + LIFE_X_OFFSET, hy + LIFE_Y_OFFSET, life);

	int pmeter = gameData->pmeter;

	int score = gameData->score;
	font->FontToSprite(hx + SCORE_X_OFFSET, hy + SCORE_Y_OFFSET, score, 7);

	int worldName = gameData->worldName;

	int countDown = gameData->GetRemainingTime();
	font->FontToSprite(hx + COUNTDOWN_X_OFFSET, hy + COUNTDOWN_Y_OFFSET, countDown);

	int coin = gameData->coin;
	font->FontToSprite(hx + COIN_X_OFFSET, hy + COIN_Y_OFFSET, coin);
}
