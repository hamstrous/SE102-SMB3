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

#define BLACK_X_OFFSET		-124
#define BLACK_Y_OFFSET		10

#define CARD_X_OFFSET		56

#define CLEAR_Y_OFFSET		30

#define GOT_Y_OFFSET		60

#define CARD_WIDTH		22
#define CARD_HEIGHT		26

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
	float hy = game->GetBackBufferHeight() - HUD_SIZE_Y - 4;

	for (int i = 0; i < 17; i++) {
		for (int j = 0; j < 4; j++) {
			sprites->Get(ID_SPRITE_BLACK)->Draw(hx + BLACK_X_OFFSET + (i * 16), hy + BLACK_Y_OFFSET + (j * 16));
		}
	}

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

	vector<int> cards = gameData->cards;
	for (int i = 0; i < cards.size(); i++)
	{
		int cardType = cards[i];
		if (cardType == -1) continue;
		sprites->Get(cardsSpriteId[cardType])->Draw(hx + CARD_X_OFFSET + (CARD_WIDTH + 2) * i, hy);
	}

	
}

void CHUD::RenderEnd()
{
	CGame* game = CGame::GetInstance();

	float midX = game->GetBackBufferWidth() / 2;
	float hy = game->GetBackBufferHeight() - HUD_SIZE_Y;


	int justWonCard = gameData->justWonCard;

	const int youGotCardXOffset = 62;
	CSprites::GetInstance()->Get(ID_SPRITE_COURSE_CLEAR)->Draw(midX, CLEAR_Y_OFFSET);
	CSprites::GetInstance()->Get(ID_SPRITE_YOU_GOT)->Draw(midX, GOT_Y_OFFSET);
	CSprites::GetInstance()->Get(cardsSpriteId[justWonCard])->Draw(midX + youGotCardXOffset, GOT_Y_OFFSET);

	int pos = gameData->cards.size() + 1;
	CAnimations::GetInstance()->Get(flickerCardsAniId[justWonCard])->Render(midX + CARD_X_OFFSET + (CARD_WIDTH + 2) * (pos - 1), hy);

}


