#include <iostream>
#include <fstream>
#include "AssetIDs.h"

#include "PlayScene.h"
#include "Camera.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Coin.h"
#include "Platform.h"
#include "Cloud.h"
#include "Floor.h"
#include "Pipe.h"
#include "Fireball.h"
#include "Plant.h"
#include "KoopaRed.h"
#include "KoopaGreen.h"
#include "ColorBlock.h"
#include "Koopa.h"
#include "QuestionBlock.h"
#include "Mushroom.h"
#include "Leaf.h"
#include "Mountain.h"
#include "Smoke.h"
#include "SampleKeyEventHandler.h"
#include "BackgroundColor.h"
#include "UnbreakableBrick.h"
#include "GenericPlatform.h"
#include "PowerUp.h"
#include "Score.h"
#include "Font.h"
#include "GameFX.h"
#include "CloudPlatform.h"
#include "GameFXManager.h"
#include "GameFX.h"
#include "TimerManager.h"
#include "Abyss.h"
#include "Block.h"
#include "Switch.h"
using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	player = NULL;
	key_handler = new CSampleKeyHandler(this);
	hud = new CHUD();

}


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_SPRITES_SCREEN 7
#define ASSETS_SECTION_SPRITES_OFFSET 8
#define ASSETS_SECTION_ANIMATIONS 2
#define ASSETS_SECTION_ANIMATIONS_VIBRATING 3
#define ASSETS_SECTION_ANIMATIONS_FLICKERING 4
#define ASSETS_SECTION_ANIMATIONS_STOPPING 5
#define ASSETS_SECTION_ANIMATIONS_BOUNCING 6

#define MAX_SCENE_LINE 1024

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPTEXTURE tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_SPRITES_SCREEN(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPTEXTURE tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex, true);
}

void CPlayScene::_ParseSection_SPRITES_OFFSET(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());
	float offsetX = (float)atof(tokens[6].c_str());
	float offsetY = (float)atof(tokens[7].c_str());

	LPTEXTURE tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex, false, offsetX, offsetY);
}

void CPlayScene::_ParseSection_ASSETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);

	LoadAssets(path.c_str());
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATIONS_VIBRATING(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation(100, 1);

	int ani_id = atoi(tokens[0].c_str());

	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATIONS_FLICKERING(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation(100, 2);

	int ani_id = atoi(tokens[0].c_str());

	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATIONS_STOPPING(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation(100, 5);

	int ani_id = atoi(tokens[0].c_str());

	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATIONS_BOUNCING(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation(100, 6);

	int ani_id = atoi(tokens[0].c_str());

	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

/*
	Parse a line in section [OBJECTS]
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	// skip invalid lines - an object set must have at least id, x, y
	if (tokens.size() < 2) return;

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x, y);
		player = (CMario*)obj;

		DebugOut(L"[INFO] Player object has been created!\n");
		break;
	case OBJECT_TYPE_GOOMBA: obj = new CGoomba(x, y); break;
	case OBJECT_TYPE_GOOMBAFLY: obj = new CGoombaFly(x, y); break;
	case OBJECT_TYPE_KOOPA_RED: obj = new CKoopaRed(x, y, atoi(tokens[3].c_str())); break;
	case OBJECT_TYPE_KOOPA_GREEN: obj = new CKoopaGreen(x, y, atoi(tokens[3].c_str())); break;
	case OBJECT_TYPE_BRICK: obj = new CBreakableBrick(x, y); break;
	case OBJECT_TYPE_COIN: obj = new CCoin(x, y, atoi(tokens[3].c_str())); break;
	case OBJECT_TYPE_FIREBALL: obj = new CFireball(x, y); break;
	case OBJECT_TYPE_SMOKE: obj = new CSmoke(x, y); break;
	case OBJECT_TYPE_COLOR_BLOCK: obj = new CColorBlock(x, y, atoi(tokens[3].c_str()), atoi(tokens[4].c_str()), atoi(tokens[5].c_str())); break;
	case OBJECT_TYPE_CLOUD: obj = new CCloud(x, y, atoi(tokens[3].c_str())); break;
	case OBJECT_TYPE_BACKGROUND_COLOR: obj = new CBackgroundColor(x, y, atoi(tokens[3].c_str()), atoi(tokens[4].c_str()), atoi(tokens[5].c_str())); break;
	case OBJECT_TYPE_UNBREAKABLEBRICK: obj = new CUnbreakableBrick(x,y, atoi(tokens[3].c_str())); break;
	case OBJECT_TYPE_CLOUDPLATFORM: obj = new CCloudPlatform(x, y); break;
	case OBJECT_TYPE_ABYSS: obj = new CAbyss(x, y); break;
	case OBJECT_TYPE_BLOCK: obj = new CBlock(x, y); break;
	case OBJECT_TYPE_SWITCH: obj = new CSwitch(x, y); break;
	case OBJECT_TYPE_FLOOR:
	{
		int width = atoi(tokens[3].c_str());
		int height = atoi(tokens[4].c_str());
		int type = atoi(tokens[5].c_str());
		obj = new CFloor(x, y, width, height, type);
		break;
	}
	case OBJECT_TYPE_PLANT:
	{
		int color = atoi(tokens[3].c_str());
		int type = atoi(tokens[4].c_str());
		int size = atoi(tokens[5].c_str());
		obj = new CPlant(x, y, color, type, size);
		break;
	}
	case OBJECT_TYPE_PLATFORM:
	{

		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());
		int blocking = atoi(tokens[9].c_str());
		obj = new CPlatform(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end, blocking
		);

		break;
	}

	case OBJECT_TYPE_PORTAL:
	{
		float r = (float)atof(tokens[3].c_str());
		float b = (float)atof(tokens[4].c_str());
		int scene_id = atoi(tokens[5].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
		break;
	}
	case OBJECT_TYPE_QUESTION_BLOCK:
	{
		int type = atoi(tokens[3].c_str());
		obj = new CQuestionBlock(x, y, type);
		break;
	}
	case OBJECT_TYPE_PIPE:
	{
		float cellWidth = (float)atof(tokens[3].c_str());
		float cellHeight = (float)atof(tokens[4].c_str());
		int height = atoi(tokens[5].c_str());
		int spriteId_top_left = atoi(tokens[6].c_str());
		int spriteId_top_right = atoi(tokens[7].c_str());
		int	spriteId_bot_left = atoi(tokens[8].c_str());
		int spriteId_bot_right = atoi(tokens[9].c_str());
		BOOLEAN isGoInside = atoi(tokens[10].c_str());
		obj = new CPipe(x, y, cellWidth, cellHeight, height, spriteId_top_left, spriteId_top_right, spriteId_bot_left, spriteId_bot_right, isGoInside);
		break;
	}
	case OBJECT_TYPE_MUSHROOM:
	{
		int type = atoi(tokens[3].c_str());
		int dir = atoi(tokens[4].c_str());
		obj = new CMushroom(x, y, type, dir);
		break;
	}
	case OBJECT_TYPE_MOUNTAIN:
	{
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		float type = (float)atof(tokens[5].c_str());
		float color = (float)atof(tokens[6].c_str());
		obj = new CMountain(x, y, cell_width, cell_height, type, color);
		break;
	}
	case OBJECT_TYPE_TAILHIT:
	{
		float type = atoi(tokens[3].c_str());
		obj = new CGameFX(x, y, type);
		break;
	}
	case OBJECT_TYPE_CAMERA:
	{
		camera = new CCamera(x,y);
		break;
	}
	default:
		DebugOut(L"[ERROR] Invalid object type: %d\n", object_type);
		return;
	}

	if(obj == NULL) return;
	// General object setup
	obj->SetPosition(x, y);

	if (CCharacter* character = dynamic_cast<CCharacter*>(obj)) {
		CCharacter* copy = character->Clone();
		characterCopy[character] = copy;
	}
	objects.push_back(obj);
}

void CPlayScene::LoadAssets(LPCWSTR assetFile)
{
	DebugOut(L"[INFO] Start loading assets from : %s \n", assetFile);

	ifstream f;
	f.open(assetFile);

	int section = ASSETS_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SPRITES]") { section = ASSETS_SECTION_SPRITES; continue; };
		if (line == "[SPRITES_SCREEN]") { section = ASSETS_SECTION_SPRITES_SCREEN; continue; };
		if (line == "[SPRITES_OFFSET]") { section = ASSETS_SECTION_SPRITES_OFFSET; continue; };
		if (line == "[ANIMATIONS]") { section = ASSETS_SECTION_ANIMATIONS; continue; };
		// many types of animation
		if (line == "[ANIMATIONS_VIBRATION]") { section = ASSETS_SECTION_ANIMATIONS_VIBRATING; continue; };
		if (line == "[ANIMATIONS_FLICKERING]") { section = ASSETS_SECTION_ANIMATIONS_FLICKERING; continue; };
		if (line == "[ANIMATIONS_STOPPING]") { section = ASSETS_SECTION_ANIMATIONS_FLICKERING; continue; };
		if (line == "[ANIMATIONS_BOUNCING]") { section = ASSETS_SECTION_ANIMATIONS_BOUNCING; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case ASSETS_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case ASSETS_SECTION_SPRITES_SCREEN: _ParseSection_SPRITES_SCREEN(line); break;
		case ASSETS_SECTION_SPRITES_OFFSET: _ParseSection_SPRITES_OFFSET(line); break;
		case ASSETS_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case ASSETS_SECTION_ANIMATIONS_VIBRATING: _ParseSection_ANIMATIONS_VIBRATING(line); break;
		case ASSETS_SECTION_ANIMATIONS_FLICKERING: _ParseSection_ANIMATIONS_STOPPING(line); break;
		case ASSETS_SECTION_ANIMATIONS_BOUNCING: _ParseSection_ANIMATIONS_BOUNCING(line); break;
		}
	}

	f.close();
	DebugOut(L"[INFO] Done loading assets from %s\n", assetFile);
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[ASSETS]") { section = SCENE_SECTION_ASSETS; continue; };
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_ASSETS: _ParseSection_ASSETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	if(camera == NULL) camera = new CCamera();
	f.close();
	CGameFXManager::GetInstance()->InitPauseFX();
	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// TO-DO: This is a "dirty" way, need a more organized way 

	//if (GetIsPause() || GetIsStop() ) return;

	if (GetIsPause()) return;

	if (GetIsStop())
	{
		for (size_t i = 0; i < objects.size(); i++)
		{
			if (dynamic_cast<CScore*>(objects[i]))
			{
				objects[i]->Update(dt, nullptr);
			}
		}
		return; 
	}
	int remainingTime = CGameData::GetInstance()->GetRemainingTime();
	if (remainingTime <= 0)
	{
		if (player != NULL)
		{	
			CMario* mario = dynamic_cast<CMario*>(player);
			mario->SetTimesUp();
			mario->SetState(MARIO_STATE_DIE);
		}
	}

	for(auto obj : objects) {
		if(CCharacter* character = dynamic_cast<CCharacter*>(obj)){
			if(dynamic_cast<CMario*>(character)) {
				continue;
			}
			if (IsObjectOutOfCamera(obj)) {
				//when out of camera, go to sleep and put back at og pos
				if (obj->GetKillOffCam()) {
					obj->Delete();
					continue;
				}
				obj->SetSleep(true);
				float nx, ny;
				if (characterCopy.find(character) != characterCopy.end() && IsObjectOutOfCamera(characterCopy[character])) {
					characterCopy[character]->GetPosition(nx, ny);
					obj->SetPosition(nx, ny);
				}else obj->SetPosition(-1, 3000); // put it out of camera
			}
			else if (obj->GetSleep()) {
				obj->SetSleep(false);
				character->Reset(characterCopy[dynamic_cast<CCharacter*>(obj)]);
			}
		}else if(CPowerUp* power = dynamic_cast<CPowerUp*>(obj)){
			if (IsObjectOutOfCamera(obj)) {
				obj->Delete();
			}
		}
		else if (CFireball* fireball = dynamic_cast<CFireball*>(obj)) {
			if (IsObjectOutOfCamera(obj)) {
				obj->Delete();
			}
		}
	}

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 0; i < objects.size(); i++)
	{
		if(!objects[i]->GetSleep()) coObjects.push_back(objects[i]);
	}

	std::sort(coObjects.begin(), coObjects.end(),
		[](LPGAMEOBJECT a, LPGAMEOBJECT b) {
			return a->IsBlocking() > b->IsBlocking(); // blocking events first
		}
	);


	for (size_t i = 0; i < objects.size(); i++)
	{
		if (!objects[i]->GetSleep()) objects[i]->Update(dt, &coObjects);
	}
	camera->Update(dt, &coObjects);
	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	PurgeDeletedObjects();

	CGameData::GetInstance()->Update(dt);

	if (deathTimer->IsDone()) {
		CGame::GetInstance()->ResetCurrentScene();
		deathTimer->Reset();
	}

}

void CPlayScene::Render()
{	

	for (auto i : objects) {
		if (i->GetSleep()) continue;

		if (dynamic_cast<CBackgroundColor*>(i))
			backgroundRenderObjects.push_back(i);

		if(dynamic_cast<CGenericPlatform*>(i)
			|| dynamic_cast<CMountain*>(i)
			|| dynamic_cast<CCloud*>(i))

			firstRenderObjects.push_back(i);
		
		if(dynamic_cast<CCharacter*>(i)
			|| dynamic_cast<CCoin*>(i)
			|| dynamic_cast<CMushroom*>(i))
			secondRenderObjects.push_back(i);

		if (dynamic_cast<CFloor*>(i)
			|| dynamic_cast<CBaseBrick*>(i)
			/*|| dynamic_cast<CPipe*>(i)*/)
			thirdRenderObjects.push_back(i);

		if (dynamic_cast<CFireball*>(i)
			|| dynamic_cast<CScore*>(i)
			|| dynamic_cast<CLeaf*>(i)
			|| dynamic_cast<CSmoke*>(i)
			|| dynamic_cast<CGameFX*>(i)
			|| dynamic_cast<CAbyss*>(i)
			|| dynamic_cast<CPipe*>(i)
			|| dynamic_cast<CGameFXManager*>(i)) {
			projectileRenderObjects.push_back(i);
		}
	}

	int mode = 0;

	CMario* mario = dynamic_cast<CMario*>(player);

	for(auto i : backgroundRenderObjects)
		i->Render();
	for (auto i : firstRenderObjects)
		i->Render();
	if (mario->GetHolding()) mario->Render();
	for (auto i : secondRenderObjects)
		if (i != player) i->Render();
	for (auto i : thirdRenderObjects)
		i->Render();
	for (auto i : projectileRenderObjects)
		i->Render();
	if (!mario->GetHolding()) mario->Render();
	

	backgroundRenderObjects.clear();
	firstRenderObjects.clear();
	secondRenderObjects.clear();
	thirdRenderObjects.clear();
	projectileRenderObjects.clear();
	hud->Render();
}

/*
*	Clear all objects from this scene
*/
void CPlayScene::Clear()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();
	for (auto i : characterCopy)
	{
		delete i.second;
	}
	characterCopy.clear();

	delete camera;
	camera = NULL;

}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well

*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();

	for (auto i : characterCopy)
	{
		if (i.second != NULL) delete i.second;
	}
	characterCopy.clear();

	player = NULL;

	deathTimer->Reset();
	stopTimer->Reset();
	pauseTimer->Reset();

	delete camera;
	camera = NULL;

	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

bool CPlayScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void CPlayScene::SetIsPause()
{	

	if (pauseTimer->IsRunning()) {

		CGameFXManager::GetInstance()->AddPause();
		CTimerManager::GetInstance()->Resume();
	}
	else {
		CGameFXManager::GetInstance()->RemovePause();
		CTimerManager::GetInstance()->Pause();
	}
	pauseTimer->Flip();
}

void CPlayScene::PurgeDeletedObjects()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		LPGAMEOBJECT o = *it;
		if (o->IsDeleted())
		{
			// delete the cocy
			if(dynamic_cast<CCharacter*>(o))
				characterCopy.erase(dynamic_cast<CCharacter*>(o));
			delete o;
			*it = NULL;
		}
	}

	// NOTE: remove_if will swap all deleted items to the end of the vector
	// then simply trim the vector, this is much more efficient than deleting individual items
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), CPlayScene::IsGameObjectDeleted),
		objects.end());
}