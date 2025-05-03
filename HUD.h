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

// Non-Charged Arrow & P
#define ID_SPRITE_ARROW_NONCHARGED        8054
#define ID_SPRITE_P_NONCHARGED            8055

// Charged Arrow & P
#define ID_SPRITE_ARROW_CHARGED           8056
#define ID_SPRITE_P_CHARGED               8057

// Course Clear
#define ID_SPRITE_COURSE_CLEAR            8058


// CONSTANTS
#define HUD_SIZE_Y	30

class CHUD : public CGameObject
{
	CGameData* gameData;
	CTimer* endTimer;

public:
	CHUD();
	void Render();
};

