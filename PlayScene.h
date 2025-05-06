#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "GoombaFly.h"

//#include "Koopas.h"


class CPlayScene: public CScene
{
protected: 
	// A play scene has to have player, right? 
	LPGAMEOBJECT player;					
	CCamera* camera;					

	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> backgroundRenderObjects;
	vector<LPGAMEOBJECT> firstRenderObjects; // background
	vector<LPGAMEOBJECT> secondRenderObjects; //
	vector<LPGAMEOBJECT> thirdRenderObjects;
	vector<LPGAMEOBJECT> projectileRenderObjects;

	CTimer* pauseTimer = new CTimer(-1), * stopTimer = new CTimer();

	vector<pair<CGameObject*, int>> addobj;
	void _ParseSection_SPRITES(string line);
	void _ParseSection_SPRITES_SCREEN(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATIONS_VIBRATING(string line);
	void _ParseSection_ANIMATIONS_FLICKERING(string line);
	void _ParseSection_ANIMATIONS_STOPPING(string line);

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
	void AddObject(CGameObject* obj) { objects.push_back(obj); }
	void Clear();
	void PurgeDeletedObjects();

	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);

	bool GetIsPause() { return pauseTimer->IsRunning(); }
	void SetIsPause() {
		pauseTimer->Flip();
	}
	bool GetIsStop() { return stopTimer->IsRunning(); }
	void SetIsStop(int timeSpan) {
		stopTimer->SetTimeSpan(timeSpan);
		stopTimer->Start();
	}

	void GetCollidableObjects(vector<LPGAMEOBJECT>* coObjects) {
		for (auto i : objects)
		{
			if (i->IsCollidable() && !i->IsDeleted())
				coObjects->push_back(i);
		}
	}
};

typedef CPlayScene* LPPLAYSCENE;

