#include "Abyss.h"



void CAbyss::Render()
{
    RenderBoundingBox();
}

void CAbyss::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void CAbyss::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = 0;
    t = y;
    r = x;
    b = y - 10;
}
