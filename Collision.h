#pragma once

#include <Windows.h>
#include <vector>
#include <algorithm>

using namespace std;

class CGameObject;
typedef CGameObject* LPGAMEOBJECT;

class CPoint;

struct CCollisionEvent;
typedef CCollisionEvent* LPCOLLISIONEVENT;

struct CCollisionEvent 
{
	LPGAMEOBJECT src_obj;		// source object : the object from which to calculate collision
	LPGAMEOBJECT obj;			// the target object
	
	float t, nx, ny;

	float dx, dy;				// *RELATIVE* movement distance between this object and obj
	bool isDeleted;		

	CCollisionEvent(float t, float nx, float ny, float dx = 0, float dy = 0, 
		LPGAMEOBJECT obj = NULL, LPGAMEOBJECT src_obj = NULL)
	{
		this->t = t;
		this->nx = nx;
		this->ny = ny;
		this->dx = dx;
		this->dy = dy;
		this->obj = obj;
		this->src_obj = src_obj;
		this->isDeleted = false;
	}

	int WasCollided();

	static bool PrioritizeBlockingEvent(const LPCOLLISIONEVENT a, const LPCOLLISIONEVENT b);
	LPCOLLISIONEVENT Reverse(LPCOLLISIONEVENT e);
};

class CCollision
{
	static CCollision* __instance;
public: 
	static void SweptAABB(
		float ml,			// move left 
		float mt,			// move top
		float mr,			// move right 
		float mb,			// move bottom
		float dx,			// 
		float dy,			// 
		float sl,			// static left
		float st,
		float sr,
		float sb,
		float& t,
		float& nx,
		float& ny);

	LPCOLLISIONEVENT SweptAABB(
		LPGAMEOBJECT objSrc, 
		DWORD dt,
		LPGAMEOBJECT objDest); 
	void Scan(
		LPGAMEOBJECT objSrc, 
		DWORD dt, 
		vector<LPGAMEOBJECT>* objDests, 
		vector<LPCOLLISIONEVENT>& coEvents, int type = 0, int dir = -1);
	void Filter(
		LPGAMEOBJECT objSrc,
		vector<LPCOLLISIONEVENT>& coEvents,
		LPCOLLISIONEVENT &colX,
		LPCOLLISIONEVENT &colY,
		int filterBlock,		
		int filterX,
		int filterY);

	void ProcessOverlap(LPGAMEOBJECT objSrc, DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void ProcessOverlapNow(LPGAMEOBJECT objSrc, DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void ProcessCollision(LPGAMEOBJECT objSrc, DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void ProcessNoBlock(LPGAMEOBJECT objSrc, DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void ProcessMarioPoints(LPGAMEOBJECT objSrc, vector<CPoint*>* points, vector<LPGAMEOBJECT>* coObjects, DWORD dt);
	int CheckStillTouchSolid(float ml, float mt, float mr, float mb, float vx, float vy, DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	bool CheckTouchingSolid(LPGAMEOBJECT obj, DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	bool CheckTouchCharacterForShellHeldHit(LPGAMEOBJECT objSrc, DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool notMario);
	bool CheckTouchCharacterForTailAttack(float ml, float mt, float mr, float mb, float vx, float vy, DWORD dt, vector<LPGAMEOBJECT>* coObjects, float x, int nx, float marioX, bool& brickHit);


	void SetPositionAndOnCollisionWith(LPGAMEOBJECT obj, float x, float y, LPCOLLISIONEVENT coEvent);

	static CCollision* GetInstance();
};