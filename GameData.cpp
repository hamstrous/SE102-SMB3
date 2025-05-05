#include "GameData.h"
#include "Game.h"
#include "PlayScene.h"
#include "Mario.h"

CGameData* CGameData::__instance = NULL;

void CGameData::Update(DWORD dt)
{
	CGame* game = CGame::GetInstance();
	CMario* mario = (CMario*) dynamic_cast<CPlayScene*>(game->GetCurrentScene())->GetPlayer();
	if(mario == NULL) return;
	float mvx , mvy;
	mario->GetSpeed(mvx, mvy);
	
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

CGameData* CGameData::GetInstance()
{
	if (__instance == NULL) __instance = new CGameData();
	return __instance;
}
