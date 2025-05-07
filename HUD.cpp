#include "HUD.h"
#include "Sprites.h"
#include "Game.h"
#include "Animations.h"
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

#define PMETER_X_OFFSET		-60
#define PMETER_Y_OFFSET		-3



CHUD::CHUD()
{
	gameData = CGameData::GetInstance();
	endTimer = new CTimer(-1);
}

void CHUD::Render()
{
	CSprites* sprites = CSprites::GetInstance();
	CAnimations* animations = CAnimations::GetInstance();
	CGame* game = CGame::GetInstance();
	CFont* font = CFont::GetInstance();

	float hx = game->GetBackBufferWidth() / 2;
	float hy = game->GetBackBufferHeight() - HUD_SIZE_Y;
	sprites->Get(ID_SPRITE_HUD)->Draw(hx, hy);

	int life = gameData->life;
	font->FontToSprite(hx + LIFE_X_OFFSET, hy + LIFE_Y_OFFSET, life);

	int pmeter = gameData->pmeter;

	for(int i = 1; i <= pmeter; i++)
	{
		if (i < 7) sprites->Get(ID_SPRITE_ARROW_CHARGED)->Draw(hx + PMETER_X_OFFSET + FONT_SIZE * (i - 1), hy + PMETER_Y_OFFSET);
		else animations->Get(ID_ANIMATION_P_CHARGED)->Render(hx + PMETER_X_OFFSET + FONT_SIZE * (i - 1) + 5, hy + PMETER_Y_OFFSET);
	}

	int score = gameData->score;
	font->FontToSprite(hx + SCORE_X_OFFSET, hy + SCORE_Y_OFFSET, score, 7);
	// 71 125 131 128
	int worldName = gameData->worldName;

	int countDown = gameData->GetRemainingTime();
	font->FontToSprite(hx + COUNTDOWN_X_OFFSET, hy + COUNTDOWN_Y_OFFSET, countDown);

	int coin = gameData->coin;
	font->FontToSprite(hx + COIN_X_OFFSET, hy + COIN_Y_OFFSET, coin);
}
