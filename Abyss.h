#pragma once
#include "GameObject.h"

#define ABYSS_BBOX_HEIGHT 16
#define ABYSS_BBOX_WIDTH 2815

class CAbyss : public CGameObject
{
protected:
public:
    CAbyss(float x, float y) : CGameObject(x, y)
    {
        this->x = x;
        this->y = y;
    }

    void Render();
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void GetBoundingBox(float& l, float& t, float& r, float& b) ; 
    int IsCollidable() override { return 1; } 
    int IsBlocking() override { return 0; }
};
