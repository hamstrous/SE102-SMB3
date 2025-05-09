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
	x = 0;
	y = 240;
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
;

void CCamera::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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
	Clamp(y, 0, 240);
}
