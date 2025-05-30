#include "GameData.h"
#include "Game.h"
#include "PlayScene.h"
#include "Mario.h"
#include "Utils.h"

CGameData* CGameData::__instance = NULL;


void CGameData::OnDeath()
{
	life--;
	CGame* game = CGame::GetInstance();

	if (life < 0) {
		life = 0;
		//game->SwitchScene(-1);
		//lose game
	}
	else {
		CGame* game = CGame::GetInstance();
	}
	pmeter = 0;
	RemoveTimers();
	marioLevel = MarioLevel::SMALL;
}

void CGameData::OnWin()
{
	CGame* game = CGame::GetInstance();
	CMario* mario = (CMario*) dynamic_cast<CPlayScene*>(game->GetCurrentScene())->GetPlayer();
	if (mario != NULL) marioLevel = mario->GetLevel();
	
	pmeter = 0;
	RemoveTimers();
	if(justWonCard != -1) 
		cards.push_back(justWonCard);
	if (cards.size() == 3) {
		if (cards[0] == cards[1] && cards[1] == cards[2])
		{
			if (cards[0] == 0) life += 5;
			else if (cards[0] == 1) life += 3;
			else life += 2;
		}
		else life += 1;
		cards.clear();
	}
	justWonCard = -1;
	fixedRemainingTime = -1;
	timeToScore = false;
}

int CGameData::GetRemainingTime()

{
	if(fixedRemainingTime != -1) return fixedRemainingTime;
	int time = levelTime - (countDown->ElapsedTime() / 1000);

	CGame* game = CGame::GetInstance();
	CTimer* pauseTimer = dynamic_cast<CPlayScene*>(game->GetCurrentScene())->GetPauseTimer();
	if (pauseTimer->IsRunning()) time = 300 - ((pauseTimer->GetTimeStart() - countDown->GetTimeStart()) / 1000);
	return (time >= 0) ? time : 0;
}

void CGameData::Update(DWORD dt)
{
	CGame* game = CGame::GetInstance();
	CMario* mario = (CMario*) dynamic_cast<CPlayScene*>(game->GetCurrentScene())->GetPlayer();
	if(mario == NULL) return;
	float mvx , mvy;
	mario->GetSpeed(mvx, mvy);
	
	if(justWonCard != -1)
	{
		int curTime = fixedRemainingTime;
		if (timeToScore && fixedRemainingTime > 0) fixedRemainingTime -= dt * SCORE_REDUCE_SPEED;
		score += (curTime - fixedRemainingTime) * 50;
		return;
	}
	if (pmeter < 7) {
		if (ptimer->IsRunning()) return;
		if (ptimer == f8) {
			if (abs(mvx) >= MARIO_RUN_MAX_SPEED_X && mario->IsOnPlatform()) {
				pmeter++;
				if (pmeter == 7) ptimer = f16;
				ptimer->Start();
			}
			else {
				if (pmeter == 0) return;
				else {
					pmeter--;
					ptimer = f24;
					ptimer->Start();
				}
			}
		}
		else if (ptimer == f24) {
			if (abs(mvx) >= MARIO_RUN_MAX_SPEED_X && mario->IsOnPlatform()) {
				pmeter++;
				if (pmeter < 7) ptimer = f8;
				else ptimer = f16;
				ptimer->Start();
			}
			else {
				if (pmeter == 0) {
					ptimer = f8;
					ptimer->Reset();
				}
				else {
					pmeter--;
					ptimer->Start();
				}
			}
		}
	}
	else {
		if (mario->IsRaccoon() && (mario->GetJumpInput() == 1 || flightMode)) {
			flightMode = true;
			if (ptimer == f255) {
				if (!ptimer->IsRunning()) {
					ptimer = f8;
					pmeter = 0;
					ptimer->Reset();
					flightMode = false;
				}
			}else ptimer = f255, ptimer->Start();
		}
		else {
			flightMode = false;
			if (!mario->IsOnPlatform()) ptimer = f255, ptimer->Start();
			if (ptimer == f16) {
				if (ptimer->IsRunning()) {
					if (abs(mvx) >= MARIO_RUN_MAX_SPEED_X && mario->GetRunInput() == 1) {
						ptimer->Start();
					}
				}
				else {
					pmeter--;
					ptimer = f24;
					ptimer->Start();
				}
			}
			else if (ptimer == f255) {
				if (ptimer->IsRunning()) {
					if (mario->IsOnPlatform() && abs(mvx) >= MARIO_RUN_MAX_SPEED_X) {
						ptimer = f16;
						ptimer->Start();
					}
					else if (mario->IsOnPlatform()) {
						pmeter--;
						ptimer = f24;
						ptimer->Start();
					}
				}
				else {
					pmeter--;
					ptimer = f24;
					ptimer->Start();
				}
			}
		}
	}
}

void CGameData::CreateTimers()
{
	// re make timers
	f24 = new CTimer(FRAME_24);
	f16 = new CTimer(FRAME_16);
	f8 = new CTimer(FRAME_8);
	f255 = new CTimer(FRAME_255);
	ptimer = f8;
	countDown = new CTimer(300000);
	countDown->Start();
}

void CGameData::RemoveTimers()
{
	if (f24 != NULL) delete f24;
	if (f16 != NULL) delete f16;
	if (f8 != NULL) delete f8;
	if (f255 != NULL) delete f255;
	if (countDown != NULL) delete countDown;
}

CGameData* CGameData::GetInstance()
{
	if (__instance == NULL) __instance = new CGameData();
	return __instance;
}
