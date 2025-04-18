#pragma once
#include "GameObject.h"
#include "AssetIDs.h"

#define TILE_SIZE 16

class CColorBlock : public CGameObject
{
protected:
	int width;				// Unit: cell 
	int height;				// Unit: cell 
    int color;				// 0: White, 1: Green, 2: Red, 3: Blue
    const int shadow[2][3] = {
		{ 1241, 1242, 1243 },
		{ 1245, 1244, 1243 }
	};
    const int colorBlocksIdSprite[4][3][3] = {
    {   // White block [0]
        { 1201, 1202, 1203 },
        { 1204, 1205, 1206 },
        { 1207, 1208, 1209 }
    },
    {   // Green block [1]
        { 1210, 1211, 1212 },
        { 1213, 1214, 1215 },
        { 1216, 1217, 1218 }
    },
    {   // Red block [2]
        { 1219, 1220, 1221 },
        { 1222, 1223, 1224 },
        { 1225, 1226, 1227 }
    },
    {   // Blue block [3]
        { 1228, 1229, 1230 },
        { 1231, 1232, 1233 },
        { 1234, 1235, 1236 }
    }
    };
    int XYtoSpriteID(int x, int y, int color)
	{
        return colorBlocksIdSprite[color][CheckCondition(x,height)][CheckCondition(y, width)];
	}

    int CheckCondition(int a, int b) {
        if (a == 0)
            return 0;
        else if (a > 0 && a < b - 1)
            return 1;
        else
            return 2;
    }

public:
	CColorBlock(float x, float y, int width, int height, int color);

	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();

    int IsDirectionColliable(float nx, float ny) {
        if (nx == 0 && ny == -1) return 1;
        else return 0;
    }
};

