#pragma once
#include "GameObject.h"
#include "Timer.h"
#include "GameData.h"

#define HUD_STATE_NORMAL	0

//for level end
#define HUD_STATE_END	1

// General HUD
#define ID_SPRITE_HUD                     8050

// Card Types
#define ID_SPRITE_CARD_MUSHROOM           8051
#define ID_SPRITE_CARD_FLOWER             8052
#define ID_SPRITE_CARD_STAR               8053

#define ID_ANI_CARD_FLICKERING_MUSHROOM     1111112
#define ID_ANI_CARD_FLICKERING_FLOWER       1111113
#define ID_ANI_CARD_FLICKERING_STAR		1111114

// Non-Charged Arrow & P
#define ID_SPRITE_ARROW_NONCHARGED        8054
#define ID_SPRITE_P_NONCHARGED            8055

// Charged Arrow & P
#define ID_SPRITE_ARROW_CHARGED           8056
#define ID_SPRITE_P_CHARGED               8057
#define ID_ANIMATION_P_CHARGED	1111111

// Course Clear
#define ID_SPRITE_COURSE_CLEAR            8058
#define ID_SPRITE_YOU_GOT            8059

// HUD BLACK
#define ID_SPRITE_BLACK            170105


// CONSTANTS
#define HUD_SIZE_Y	30

class CHUD : public CGameObject
{
	CGameData* gameData;
	CTimer* tempTimer;
	int flickerCardsAniId[3] = { ID_ANI_CARD_FLICKERING_STAR, ID_ANI_CARD_FLICKERING_FLOWER, ID_ANI_CARD_FLICKERING_MUSHROOM };
	int cardsSpriteId[3] = { ID_SPRITE_CARD_STAR, ID_SPRITE_CARD_FLOWER, ID_SPRITE_CARD_MUSHROOM };

public:
	CHUD();
	~CHUD(){
		if (tempTimer != NULL)
			delete tempTimer;
	}
	void Render();
	void RenderEnd();
};

