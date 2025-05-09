#include "GameObject.h"
class CBlock : public CGameObject
{
protected:
public:
    CBlock(float x, float y) : CGameObject(x, y)
    {
        this->x = x;
        this->y = y;
    }

    void Render();
    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {};
    void GetBoundingBox(float& l, float& t, float& r, float& b);
    int IsCollidable() override { return 1; }
    int IsBlocking() override { return 1; }
};


