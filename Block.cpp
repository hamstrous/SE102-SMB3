#include "Block.h"

void CBlock::Render()
{
    RenderBoundingBox();
}

void CBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = 0;
    t = 0;
    r = x;
    b = t + y;
}
