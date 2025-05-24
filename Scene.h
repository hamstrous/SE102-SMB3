#pragma once

#include "KeyEventHandler.h"
#include "Camera.h"
#include "HUD.h"

/*
*  Abstract class for a game scene
*/
class CScene
{
protected:
	LPKEYEVENTHANDLER key_handler;
	int id;
	LPCWSTR sceneFilePath;
	CHUD* hud;

public: 
	virtual ~CScene() { 
		if (key_handler != NULL) delete key_handler; 
		if (hud != NULL) delete hud; 
		if(sceneFilePath != NULL) delete[] sceneFilePath;
	}
	CScene(int id, LPCWSTR filePath)
	{
		this->id = id;
		this->sceneFilePath = filePath;
		this->key_handler = NULL;
		this->hud = NULL;
	}

	LPKEYEVENTHANDLER GetKeyEventHandler() { return key_handler; }
	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0; 
	virtual CCamera* GetCamera() = 0; 
};
typedef CScene * LPSCENE;


class CSceneKeyHandler : public CKeyEventHandler
{
protected: 
	CScene * scene; 

public:
	virtual void KeyState(BYTE *states) = 0;
	virtual void OnKeyDown(int KeyCode) = 0;
	virtual void OnKeyUp(int KeyCode) = 0;
	CSceneKeyHandler(LPSCENE s) :CKeyEventHandler() { scene = s; }
};

typedef CSceneKeyHandler* LPSCENEKEYHANDLER;