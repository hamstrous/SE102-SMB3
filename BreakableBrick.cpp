#include "BreakableBrick.h"
#include "ScoreManager.h"
void CBreakableBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_BREAKABLEBRICK)->Render(x, y);
	//RenderBoundingBox();
}

void CBreakableBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x - BRICK_BBOX_WIDTH/2;
	t = y - BRICK_BBOX_HEIGHT/2;
	r = l + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;
}

void CBreakableBrick::SideHit()
{
	isDeleted = true;
	CScoreManager::GetInstance()->AddScore(x, y, 10);
}

void CBreakableBrick::BottomHit()
{

}
