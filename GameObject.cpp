#include <d3dx9.h>
#include <algorithm>


#include "debug.h"
#include "Textures.h"
#include "Game.h"
#include "GameObject.h"
#include "Sprites.h"
#include "Game.h"
#include "PlayScene.h"

CGameObject::CGameObject()
{
	x = y = 0;
	vx = vy = 0;
	nx = 1;	
	state = -999;
	isDeleted = false;
}

void CGameObject::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l,t,r,b; 

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float cx, cy; 
	CGame::GetInstance()->GetCurrentScene()->GetCamera()->GetPosition(cx, cy);

	CGame::GetInstance()->Draw(x - cx, y - cy, bbox, &rect, BBOX_ALPHA);
}

void CGameObject::GetCollidableObjects(vector<LPGAMEOBJECT>* coObjects)
{
	dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene())->GetCollidableObjects(coObjects);
}

CGameObject::~CGameObject()
{

}

bool CGameObject::GetIsPause()
{
	CGame* game = CGame::GetInstance();
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	return scene->GetIsPause();
}
void CGameObject::SetIsStop()
{
	CGame* game = CGame::GetInstance();
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	scene->SetIsStop(1);
}
bool CGameObject::GetIsStop()
{
	CGame* game = CGame::GetInstance();
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	return scene->GetIsStop();
}

bool CGameObject::GetIsDead()
{
	CGame* game = CGame::GetInstance();
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	return scene->GetIsDead();
}
