#pragma once
#include "GameObject.h"
#include "AssetIDs.h"

class CFloor : public CGameObject
{
protected:
    int width;				// Unit: cell 
    int height;				// Unit: cell 
    int type;				// 0: Floor, 1: Orange, 2: Green

    const int idSprite[4][2][3] = {
        {
            { 1301, 1302, 1303 },
            { 1304, 1305, 1306 }
        },
		{
			{ 1307, 1308, 1309 },
			{ 1310, 1311, 1312 }
		},
		{
			{ 1313, 1314, 1315 },
			{ 1316, 1317, 1318 }
		}
    };

    int XYtoSpriteID(int x, int y, int type)
    {
        return idSprite[type][CheckCondition(x, height, false)][CheckCondition(y, width, true)];
    }

    int CheckCondition(int a, int b, bool fullBlock) {\
        if (fullBlock) {
            if (a == 0)
                return 0;
            else if (a > 0 && a < b - 1)
                return 1;
            else
                return 2;
        }
        else {
            if(a == 0)
				return 0;
			else return 1;        }
    }

public:
    CFloor(float x, float y, int width, int height, int type);

    void Render();
    void Update(DWORD dt) {}
    void GetBoundingBox(float& l, float& t, float& r, float& b);
    void RenderBoundingBox();
    virtual int IsCollidable() { return 1; };

};

