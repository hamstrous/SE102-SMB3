#include "Abyss.h"


void CAbyss::Render()
{
    //RenderBoundingBox();
}

void CAbyss::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    CGameObject::Update(dt, coObjects);
    CCollision::GetInstance()->ProcessCollision(this, dt, coObjects);
}

void CAbyss::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x - ABYSS_BBOX_WIDTH /2;
    t = y - ABYSS_BBOX_HEIGHT / 2;
    r = l + ABYSS_BBOX_WIDTH;
    b = t + ABYSS_BBOX_HEIGHT;
}
