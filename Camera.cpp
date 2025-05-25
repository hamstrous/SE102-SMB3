#include "Camera.h"
#include "Game.h"
#include "PlayScene.h"
#include "Utils.h"

CMario* CCamera::GetMario()
{
	CPlayScene* scene = (CPlayScene*) CGame::GetInstance()->GetCurrentScene();
	CMario* mario = dynamic_cast<CMario*>(scene->GetPlayer());
	return mario;

}

CCamera::CCamera() : CGameObject(0, 0) {
	int shakeStart = -1;
	CGame* game = CGame::GetInstance();
	screenHeight = game->GetBackBufferHeight();
	screenWidth = game->GetBackBufferWidth();
}
CCamera::CCamera(float x, float y) : CGameObject(x, y)
{
	CGame* game = CGame::GetInstance();
	screenHeight = game->GetBackBufferHeight();
	screenWidth = game->GetBackBufferWidth();
}
CCamera::CCamera(float x, float y, float levelWidth, float levelHeight, float state) : CGameObject(x, y)
{
	CGame* game = CGame::GetInstance();
	screenHeight = game->GetBackBufferHeight();
	screenWidth = game->GetBackBufferWidth();
	this->x = x;
	this->y = y;
	this->levelWidth = levelWidth;
	this->levelHeight = levelHeight;
	SetState(state);
}
;

void CCamera::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == CAMERA_STATE_FOLLOW) {
		UpdateFollow(dt, coObjects);
	}
	else if (state == CAMERA_STATE_MOVING) {
		UpdateMoving(dt, coObjects);
	}
	else if(state == CAMERA_STATE_SECRET_ROOM) {
		UpdateSecretRoom(dt, coObjects);
	}
	else if (state == CAMERA_STATE_1_4_END)
	{
		Update1_4End(dt, coObjects);
	}
}

void CCamera::UpdateFollow(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CMario* mario = GetMario();
	if (mario == NULL) return;

	float mx, my;
	mario->GetPosition(mx, my);

	float cl, cr, ct, cb; //line to check if we need to move the camera

	cl = x + screenWidth / 2 - FIXED_BOX_SIZE;
	cr = x + screenWidth / 2 + FIXED_BOX_SIZE;
	ct = y + screenHeight / 2 - FIXED_BOX_SIZE;
	cb = y + screenHeight / 2 + FIXED_BOX_SIZE;

	if (mx < cl) x -= cl - mx;
	if (mx > cr) x += mx - cr;
	if (my > cb) y += my - cb;
	if (CGameData::GetInstance()->IsFlightMode()) {
		if (my < ct) y -= ct - my;
	}
	Clamp(x, 0, levelWidth - screenWidth);
	Clamp(y, 0, levelHeight - screenHeight);
	if(GetTickCount64() - shakeStart <= SHAKY_TIME) y += (GetTickCount64() & 1) * 2;
}

void CCamera::UpdateMoving(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	x += vx * dt;
	if(x > levelWidth - screenWidth)
	{
		SetState(CAMERA_STATE_FOLLOW);
	}
	Clamp(x, 0, levelWidth - screenWidth);
	Clamp(y, 0, levelHeight - screenHeight);
}

void CCamera::UpdateSecretRoom(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CMario* mario = GetMario();
	if (mario == NULL) return;

	float mx, my;
	mario->GetPosition(mx, my);

	float cl, cr, ct, cb; //line to check if we need to move the camera

	cl = x + screenWidth / 2 - FIXED_BOX_SIZE;
	cr = x + screenWidth / 2 + FIXED_BOX_SIZE;

	if (mx < cl) x -= cl - mx;
	if (mx > cr) x += mx - cr;
}

void CCamera::Update1_4End(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CMario* mario = GetMario();
	if (mario == NULL) return;

	float mx, my;
	mario->GetPosition(mx, my);

	float cl, cr, ct, cb; //line to check if we need to move the camera

	cl = x + screenWidth / 2 - FIXED_BOX_SIZE;
	cr = x + screenWidth / 2 + FIXED_BOX_SIZE;
	ct = y + screenHeight / 2 - FIXED_BOX_SIZE;
	cb = y + screenHeight / 2 + FIXED_BOX_SIZE;

	if (mx < cl) x -= cl - mx;
	if (mx > cr) x += mx - cr;
	if (my > cb) y += my - cb;
	if (CGameData::GetInstance()->IsFlightMode()) {
		if (my < ct) y -= ct - my;
	}
	Clamp(x, MAX_X_CAMERA_1_4, levelWidth2 - screenWidth);
	Clamp(y, 0, levelHeight - screenHeight);
}
