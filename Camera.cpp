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
	x = y = 0;
	CGame* game = CGame::GetInstance();
	screenHeight = game->GetBackBufferHeight();
	screenWidth = game->GetBackBufferWidth();
};

void CCamera::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	int moveX, moveY;
	IsMarioOutsideOfCameraFixedBox(moveX, moveY);
	x += moveX * CAMERA_SPEED;
	y += moveY * CAMERA_SPEED;
	Clamp(x, 0, levelWidth - screenWidth);
	Clamp(y, 0, levelHeight - screenHeight);
}

void CCamera::GetNewCameraPosBasedOnMario(float& cx, float& cy)
{
	float nx, ny;
	CMario* mario = GetMario();
	if(mario == NULL) 	{
		cx = x;
		cy = y;
		return;
	}
	// Get the position of the Mario
	mario->GetPosition(nx, ny);
	// Get the position of the camera
	nx -= screenWidth / 2;
	ny -= screenHeight / 2;

	//return value
	cx = nx;
	cy = ny;

}

void CCamera::IsMarioOutsideOfCameraFixedBox(int &moveX, int &moveY)
{
	moveX = moveY = 0;
	CMario* mario = GetMario();
	if(mario == NULL) return;	

	float mx, my;
	mario->GetPosition(mx, my);

	float cl, cr, ct, cb; //line to check if we need to move the camera

	cl = x + screenWidth / 2 - FIXED_BOX_SIZE;
	cr = x + screenWidth / 2 + FIXED_BOX_SIZE;
	ct = y + screenHeight / 2 - FIXED_BOX_SIZE;
	cb = y + screenHeight / 2 + FIXED_BOX_SIZE;

	if (mx < cl) moveX = -1;
	if (mx > cr) moveX = 1;
	if (my < ct) moveY = -1;
	if (my > cb) moveY = 1;
}
