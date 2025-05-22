#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Mario.h"
#include "Timer.h"

//#include "Koopas.h"

#define DEAD_TIME	3000
#define WIN_TIME	3000
#define FADE_TIME	800

class CPlayScene : public CScene
{
protected:
	const float fadeoutSpeed = 0.002f;

	// A play scene has to have player, right? 
	LPGAMEOBJECT player;
	CCamera* camera;

	unordered_map<CCharacter*, CCharacter*> characterCopy;
	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> backgroundRenderObjects;
	vector<LPGAMEOBJECT> firstRenderObjects; // background
	vector<LPGAMEOBJECT> secondRenderObjects; //
	vector<LPGAMEOBJECT> thirdRenderObjects;
	vector<LPGAMEOBJECT> projectileRenderObjects;

	CTimer* pauseTimer = new CTimer(-1), * stopTimer = new CTimer();
	CTimer* deathTimer = new CTimer(DEAD_TIME);
	CTimer* winTimer = new CTimer(WIN_TIME);

	float fadeoutAlpha = 1;

	void _ParseSection_SPRITES(string line);
	void _ParseSection_SPRITES_SCREEN(string line);
	void _ParseSection_SPRITES_OFFSET(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATIONS_VIBRATING(string line);
	void _ParseSection_ANIMATIONS_FLICKERING(string line);
	void _ParseSection_ANIMATIONS_STOPPING(string line);
	void _ParseSection_ANIMATIONS_BOUNCING(string line);
	void _ParseSection_ASSETS(string line);
	void _ParseSection_OBJECTS(string line);

	void LoadAssets(LPCWSTR assetFile);

public:
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	LPGAMEOBJECT GetPlayer() { return player; }
	CCamera* GetCamera() { return camera; }
	CTimer* GetPauseTimer() { return pauseTimer; }
	void SetPlayer(LPGAMEOBJECT player) { this->player = player; }
	void AddObject(CGameObject* obj) { objects.push_back(obj); }
	void Clear();
	void PurgeDeletedObjects();

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);

	bool GetIsPause() { return pauseTimer->IsRunning(); }
	void SetIsPause();

	bool GetIsStop() { return stopTimer->IsRunning(); }
	void SetIsStop(ULONGLONG timeSpan) {
		stopTimer->SetTimeSpan(timeSpan);
		stopTimer->Start();
	}

	bool GetIsDead() { return deathTimer->IsRunning(); }

	void GetCollidableObjects(vector<LPGAMEOBJECT>* coObjects) {
		for (auto i : objects)
		{
			if (i->IsCollidable() && !i->IsDeleted())
				coObjects->push_back(i);
		}
	}

	bool IsObjectOutOfCamera(LPGAMEOBJECT obj) {
		return camera->IsOutOfCamera(obj);
	}

	void OnPlayerDeath() {
		deathTimer->Start();
		camera->SetState(CAMERA_STATE_STOP);
	}

	void OnPlayerWin(int card) {
		((CMario*)player)->WinCutscene();
		CGameData::GetInstance()->SetJustWonCard(card);
	}

	void GetCamPos(float& x, float& y) {
		camera->GetPosition(x, y);
	}

	void GetPlayerPosition(float& x, float& y) {
		if (player != NULL)
			player->GetPosition(x, y);
		else
			x = y = 0;
	}
	void SetPlayerPosition(float x, float y)
	{
		CMario* mario = dynamic_cast<CMario*>(player);
		mario->SetPosition(x, y);
	}

	void ScreenTransition() {
		if (fadeoutAlpha > 1) {
			fadeoutAlpha = 1;
		}
		if(fadeoutAlpha < 0) {
			fadeoutAlpha = 0;
		}

		float w, h;
		h = CGame::GetInstance()->GetBackBufferHeight();
		w = CGame::GetInstance()->GetBackBufferWidth();

		float x, y;
		x = w / 2;
		y = h / 2;

		D3DXVECTOR3 p(x, y, 0);
		RECT rect;

		LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_TRANSITION);

		float l, t, r, b;

		l = x - w / 2;
		t = y - h / 2;
		r = l + w;
		b = t + h;

		rect.left = 0;
		rect.top = 0;
		rect.right = (int)r - (int)l;
		rect.bottom = (int)b - (int)t;

		

		CGame::GetInstance()->Draw(x, y, bbox, &rect, fadeoutAlpha);
	}

	bool IsBackgroundObject(LPGAMEOBJECT obj);
};


typedef CPlayScene* LPPLAYSCENE;

