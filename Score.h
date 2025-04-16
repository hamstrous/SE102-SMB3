#pragma once
#include "GameObject.h"
#include "Game.h"
#include "PlayScene.h"


#define ID_ANI_SCORE_100 9000
#define ID_ANI_SCORE_200 9001
#define ID_ANI_SCORE_400 9002
#define ID_ANI_SCORE_800 9003
#define ID_ANI_SCORE_1000 9004
#define ID_ANI_SCORE_2000 9005
#define ID_ANI_SCORE_4000 9006
#define ID_ANI_SCORE_8000 9007

#define SCORE_SPEED 0.015f
#define SCORE_SPEED2 0.03f

#define SCORE_DISTANCE_UP 10
#define SCORE_DISTANCE_UP2 5

#define SCORE_STATE_INIT 100
#define SCORE_STATE_UP 200
#define SCORE_STATE_UP2 300
#define SCORE_STATE_DONE 400
class CScore : public CGameObject
{
protected:
	int score;
	float  startY;
public:
	CScore(float x, float y, int score) : CGameObject()
	{	
		this->x = x;
		this->y = y;
		this->score = score;
		startY = y;
		SetState(SCORE_STATE_INIT);
	}
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) { }
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void SetState(int state);
	~CScore() {};
};

