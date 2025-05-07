#include "Collision.h"
#include "GameObject.h"
#include "Mario.h"
#include "Mushroom.h"
#include "Character.h"
#include "BaseBrick.h"
#include "GenericPlatform.h"
#include "GameFXManager.h"
#include "debug.h"
#include "Mario.h"

#define BLOCK_PUSH_FACTOR 0.01f

CCollision* CCollision::__instance = NULL;

static bool IsColliding(float al, float at, float ar, float ab, float bl, float bt, float br, float bb) {
	return al < br && ar > bl && at < bb && ab > bt;
}

static int StillColliding(float al, float at, float ar, float ab, float dx, float dy, float bl, float bt, float br, float bb) {
	int curTouch = IsColliding(al, at, ar, ab, bl, bt, br, bb);
	int nexTouch = IsColliding(al + dx, at + dy, ar + dx, ab + dy, bl, bt, br, bb);
	if(curTouch == 0) return 0; // not touch
	return  curTouch + nexTouch; // 2: still touch, 1: touch then not touch
}

int CCollisionEvent::WasCollided() {
	return
		t >= 0.0f && t <= 1.0f && obj->IsDirectionColliable(nx, ny) == 1;
}

CCollision* CCollision::GetInstance()
{
	if (__instance == NULL) __instance = new CCollision();
	return __instance;
}

/*
	SweptAABB 
*/
void CCollision::SweptAABB(
	float ml, float mt, float mr, float mb,
	float dx, float dy,
	float sl, float st, float sr, float sb,
	float& t, float& nx, float& ny)
{

	float dx_entry, dx_exit, tx_entry, tx_exit;
	float dy_entry, dy_exit, ty_entry, ty_exit;

	float t_entry;
	float t_exit;

	t = -1.0f;			// no collision
	nx = ny = 0;

	float n_ml = ml + dx;
	float n_mt = mt + dy;
	float n_mr = mr + dx;
	float n_mb = mb + dy; 

	//if (isColliding(ml,mt,mr,mb,sl,st,sr,sb) && isColliding(n_ml,n_mt,n_mr,n_mb,sl,st,sr,sb)){
	//	t = 0;
	//	return;
	//}// if moving inside (collide before and after)

	//
	// Broad-phase test 
	//

	float bl = dx > 0 ? ml : ml + dx;
	float bt = dy > 0 ? mt : mt + dy;
	float br = dx > 0 ? mr + dx : mr;
	float bb = dy > 0 ? mb + dy : mb;

	if (br < sl || bl > sr || bb < st || bt > sb) return;


	if (dx == 0 && dy == 0) return;		// moving object is not moving > obvious no collision

	if (dx > 0)
	{
		dx_entry = sl - mr;
		dx_exit = sr - ml;
	}
	else if (dx < 0)
	{
		dx_entry = sr - ml;
		dx_exit = sl - mr;
	}


	if (dy > 0)
	{
		dy_entry = st - mb;
		dy_exit = sb - mt;
	}
	else if (dy < 0)
	{
		dy_entry = sb - mt;
		dy_exit = st - mb;
	}

	if (dx == 0)
	{
		tx_entry = -9999999.0f;
		tx_exit = 99999999.0f;
	}
	else
	{
		tx_entry = dx_entry / dx;
		tx_exit = dx_exit / dx;
	}

	if (dy == 0)
	{
		ty_entry = -99999999999.0f;
		ty_exit = 99999999999.0f;
	}
	else
	{
		ty_entry = dy_entry / dy;
		ty_exit = dy_exit / dy;
	}


	if ((tx_entry < 0.0f && ty_entry < 0.0f) || tx_entry > 1.0f || ty_entry > 1.0f) return;

	t_entry = max(tx_entry, ty_entry);
	t_exit = min(tx_exit, ty_exit);

	if (t_entry > t_exit) return;

	t = t_entry;

	if (tx_entry > ty_entry)
	{
		ny = 0.0f;
		dx > 0 ? nx = -1.0f : nx = 1.0f;
	}
	else
	{
		nx = 0.0f;
		dy > 0 ? ny = -1.0f : ny = 1.0f;
	}
}

/*
	Extension of original SweptAABB to deal with two moving objects
*/
LPCOLLISIONEVENT CCollision::SweptAABB(LPGAMEOBJECT objSrc, DWORD dt, LPGAMEOBJECT objDest)
{
	float sl, st, sr, sb;		// static object bbox
	float ml, mt, mr, mb;		// moving object bbox
	float t, nx, ny;

	float mvx, mvy;
	objSrc->GetSpeed(mvx, mvy);
	float mdx = mvx * dt;
	float mdy = mvy * dt;

	float svx, svy;
	objDest->GetSpeed(svx, svy);
	float sdx = svx * dt;
	float sdy = svy * dt;

	//
	// NOTE: new m speed = original m speed - collide object speed
	// 
	float dx = mdx - sdx;
	float dy = mdy - sdy;

	objSrc->GetBoundingBox(ml, mt, mr, mb);
	objDest->GetBoundingBox(sl, st, sr, sb);

	SweptAABB(
		ml, mt, mr, mb,
		dx, dy,
		sl, st, sr, sb,
		t, nx, ny
	);

	CCollisionEvent* e = new CCollisionEvent(t, nx, ny, dx, dy, objDest, objSrc);
	return e;
}

/*
	Calculate potential collisions with the list of colliable objects

	coObjects: the list of colliable objects
	coEvents: list of potential collisions
*/
void CCollision::Scan(LPGAMEOBJECT objSrc, DWORD dt, vector<LPGAMEOBJECT>* objDests, vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < objDests->size(); i++)
	{
		if (!(objDests->at(i)->IsCollidable())) continue; // if the other obj not collidable then skip (2 way)
		LPCOLLISIONEVENT e = SweptAABB(objSrc, dt, objDests->at(i));

		if (e->WasCollided()==1)
			coEvents.push_back(e);
		else
			delete e;
	}

	//std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void CCollision::Filter( LPGAMEOBJECT objSrc,
	vector<LPCOLLISIONEVENT>& coEvents,
	LPCOLLISIONEVENT &colX,
	LPCOLLISIONEVENT &colY,
	int filterBlock = 1,		// 1 = only filter block collisions, 0 = filter all collisions 
	int filterX = 1,			// 1 = process events on X-axis, 0 = skip events on X 
	int filterY = 1)			// 1 = process events on Y-axis, 0 = skip events on Y
{
	float min_tx, min_ty;

	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];
		if (c->isDeleted) continue;
		if (c->obj->IsDeleted()) continue; 

		// ignore collision event with object having IsBlocking = 0 (like coin, mushroom, etc)
		if (filterBlock == 1 && !c->obj->IsBlocking()) 
		{
			continue;
		}

		if (c->t < min_tx && c->nx != 0 && filterX == 1) {
			min_tx = c->t; min_ix = i;
		}

		if (c->t < min_ty && c->ny != 0 && filterY == 1) {
			min_ty = c->t; min_iy = i;
		}
	}

	if (min_ix >= 0) colX = coEvents[min_ix];
	if (min_iy >= 0) colY = coEvents[min_iy];
}

/*
*  Simple/Sample collision framework 
*  NOTE: Student might need to improve this based on game logic 
*/
void CCollision::Process(LPGAMEOBJECT objSrc, DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vector<LPCOLLISIONEVENT> coEvents;
	LPCOLLISIONEVENT colX = NULL; 
	LPCOLLISIONEVENT colY = NULL;

	coEvents.clear();

	if (objSrc->IsCollidable())
	{
		Scan(objSrc, dt, coObjects, coEvents);
	}

	// No collision detected
	if (coEvents.size() == 0)
	{
		objSrc->OnNoCollision(dt);
	}
	else
	{
		Filter(objSrc, coEvents, colX, colY);

		float x, y, vx, vy, dx, dy;
		objSrc->GetPosition(x, y);
		objSrc->GetSpeed(vx, vy);
		dx = vx * dt;
		dy = vy * dt;
		if (colX != NULL && colY != NULL) 
		{
			if (colY->t < colX->t)	// was collision on Y first ?
			{
				y += colY->t * dy + colY->ny * BLOCK_PUSH_FACTOR;
				SetPositionAndOnCollisionWith(objSrc, x, y, colY); // set position and call OnCollisionWith

				//
				// see if after correction on Y, is there still a collision on X ? 
				//
				LPCOLLISIONEVENT colX_other = NULL;

				//
				// check again if there is true collision on X 
				//
				colX->isDeleted = true;		// remove current collision event on X

				// replace with a new collision event using corrected location 
				coEvents.push_back(SweptAABB(objSrc, dt, colX->obj));

				// re-filter on X only
				Filter(objSrc, coEvents, colX_other, colY, /*filterBlock = */ 1, 1, /*filterY=*/0);

				if (colX_other != NULL)
				{
					x += colX_other->t * dx +colX_other->nx * BLOCK_PUSH_FACTOR;
					SetPositionAndOnCollisionWith(objSrc, x, y, colX_other); // set position and call OnCollisionWith
				}
				else
				{
					x += dx;
					objSrc->SetPosition(x, y);
				}
			}
			else // collision on X first
			{
				x += colX->t * dx + colX->nx * BLOCK_PUSH_FACTOR;
				SetPositionAndOnCollisionWith(objSrc, x, y, colX); // set position and call OnCollisionWith

				//
				// see if after correction on X, is there still a collision on Y ? 
				//
				LPCOLLISIONEVENT colY_other = NULL;

				//
				// check again if there is true collision on Y
				//
				colY->isDeleted = true;		// remove current collision event on Y

				// replace with a new collision event using corrected location 
				coEvents.push_back(SweptAABB(objSrc, dt, colY->obj));

				// re-filter on Y only
				Filter(objSrc, coEvents, colX, colY_other, /*filterBlock = */ 1, /*filterX=*/0, /*filterY=*/1);

				if (colY_other != NULL)
				{
					y += colY_other->t * dy + colY_other->ny * BLOCK_PUSH_FACTOR;
					SetPositionAndOnCollisionWith(objSrc, x, y, colY_other); // set position and call 
				}
				else
				{
					y += dy;
					objSrc->SetPosition(x, y);
				}
			}
		}
		else
		if (colX != NULL)
		{
			x += colX->t * dx + colX->nx * BLOCK_PUSH_FACTOR;
			y += dy;
			SetPositionAndOnCollisionWith(objSrc, x, y, colX); // set position and call OnCollisionWith
		}
		else 
			if (colY != NULL)
			{
				x += dx;
				y += colY->t * dy + colY->ny * BLOCK_PUSH_FACTOR;
				SetPositionAndOnCollisionWith(objSrc, x, y, colY); // set position and call OnCollisionWith
			}
			else // both colX & colY are NULL 
			{
				x += dx;
				y += dy;
				objSrc->SetPosition(x, y);
			}
		
	}

	//
	// Scan all non-blocking collisions for further collision logic
	//
	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT e = coEvents[i];
		if (e->isDeleted) continue;
		if (e->obj->IsBlocking()) continue;  // blocking collisions were handled already, skip them

		objSrc->OnCollisionWith(e);			
	}


	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

// the start of mario seperate collision check (enemy, item touch, collsion point check)
void CCollision::ProcessForMario(LPGAMEOBJECT objSrc, vector<LPGAMEOBJECT>* points, vector<LPGAMEOBJECT>* coObjects, vector<bool>* pointsTouched)
{
	pointsTouched->clear();
	int k = 0;
	float ox, oy;
	(*points)[DOWNLEFT]->GetOldPosition(ox, oy);
	for (auto i : *points) {
		LPGAMEOBJECT point = i;
		bool touched = false;
		
		for (auto obj : *coObjects)
		{
			if (obj->IsBlocking())
			{
				float sl, st, sr, sb;
				obj->GetBoundingBox(sl, st, sr, sb);
				float ml, mt, mr, mb;
				point->GetBoundingBox(ml, mt, mr, mb);

				if(k == LEFTUP || k == LEFTDOWN || k == RIGHTUP || k == RIGHTDOWN)
					if(dynamic_cast<CGenericPlatform*>(obj) && oy > st) continue; // skip if moving up to platform

				if (IsColliding(ml, mt, mr, mb, sl, st, sr, sb))
				{
					pointsTouched->push_back(true);
					touched = true;
					break;
				}
			}
		}
		if(!touched) pointsTouched->push_back(false);
		k++;
	}
}

// 2: still touch, 1: touch then not touch, 0: not touch
int CCollision::CheckStillTouchSolid(float ml, float mt, float mr, float mb, float vx, float vy, DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	int mxOutCome = 0;
	if (coObjects->size() > 0)
	{
		for (auto obj : *coObjects)
		{
			if (obj->IsBlocking())
			{
				float sl, st, sr, sb;
				obj->GetBoundingBox(sl, st, sr, sb);
				float mdx = vx * dt;
				float mdy = vy * dt;

				float svx, svy;
				obj->GetSpeed(svx, svy);
				float sdx = svx * dt;
				float sdy = svy * dt;

				//
				// NOTE: new m speed = original m speed - collide object speed
				// 
				float dx = mdx - sdx;
				float dy = mdy - sdy;

				mxOutCome = max(mxOutCome, StillColliding(ml, mt, mr, mb, dx, dy, sl, st, sr, sb));
			}
		}
	}return mxOutCome;
}

bool CCollision::CheckTouchingSolid(float ml, float mt, float mr, float mb, float vx, float vy, DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	int mxOutCome = 0;
	if (coObjects->size() > 0)
	{
		for (auto obj : *coObjects)
		{
			if (dynamic_cast<CBaseBrick*>(obj))
			{
				float sl, st, sr, sb;
				obj->GetBoundingBox(sl, st, sr, sb);
				float mdx = vx * dt;
				float mdy = vy * dt;

				float svx, svy;
				obj->GetSpeed(svx, svy);
				float sdx = svx * dt;
				float sdy = svy * dt;

				//
				// NOTE: new m speed = original m speed - collide object speed
				// 
				float dx = mdx - sdx;
				float dy = mdy - sdy;

				if(IsColliding(ml + dx, mt + dy, mr + dx, mb + dy, sl, st, sr, sb)) return true;
			}
		}
	}return false;
}

bool CCollision::CheckTouchCharacterForShellHeldHit(float ml, float mt, float mr, float mb, float vx, float vy, DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool notMario = 1)
{
	bool isTouching = false;
	if (coObjects->size() > 0)
	{
		for (auto obj : *coObjects)
		{
			if(notMario && dynamic_cast<CMario*>(obj)) continue;
			if(dynamic_cast<CCharacter*>(obj) == NULL) continue;
			CCharacter* character = dynamic_cast<CCharacter*>(obj);
			if (obj->IsCollidable() )
			{
				float sl, st, sr, sb;
				obj->GetBoundingBox(sl, st, sr, sb);
				float mdx = vx * dt;
				float mdy = vy * dt;

				float svx, svy;
				obj->GetSpeed(svx, svy);
				float sdx = svx * dt;
				float sdy = svy * dt;

				//
				// NOTE: new m speed = original m speed - collide object speed
				// 
				float dx = mdx - sdx;
				float dy = mdy - sdy;

				if (IsColliding(ml + dx, mt + dy, mr + dx, mb + dy, sl, st, sr, sb)) {
					isTouching = true;
					character->ShellHeldHit((ml+mr)/2);
				}
			}
		}
	}return isTouching;
}

bool CCollision::CheckTouchCharacterForTailAttack(float ml, float mt, float mr, float mb, float vx, float vy, DWORD dt, vector<LPGAMEOBJECT>* coObjects, float x, int nx, float marioX)
{
	bool isTouching = false;
	if (coObjects->size() > 0)
	{
		for (auto obj : *coObjects)
		{
			if (dynamic_cast<CMario*>(obj)) continue;
			if (obj->IsCollidable())
			{
				float sl, st, sr, sb;
				obj->GetBoundingBox(sl, st, sr, sb);
				float mdx = vx * dt;
				float mdy = vy * dt;

				float svx, svy;
				obj->GetSpeed(svx, svy);
				float sdx = svx * dt;
				float sdy = svy * dt;

				//
				// NOTE: new m speed = original m speed - collide object speed
				// 
				float dx = mdx - sdx;
				float dy = mdy - sdy;

				if (IsColliding(ml + dx, mt + dy, mr + dx, mb + dy, sl, st, sr, sb)) {
					isTouching = true;
					if (CCharacter* character = dynamic_cast<CCharacter*>(obj))
					{
						character->TailHit(x);
						CGameFXManager::GetInstance()->AddGameFX((ml + mr) / 2, (mt + mb) / 2, 1);

					}
					else if (CBaseBrick* brick = dynamic_cast<CBaseBrick*>(obj))
					{	
						if (nx > 0)
						{	
							brick->SideHit();
						}
						
					}
				}
			}
		}
	}return isTouching;
}

void CCollision::SetPositionAndOnCollisionWith(LPGAMEOBJECT obj, float x, float y, LPCOLLISIONEVENT coEvent)
{
	obj->SetPosition(x, y);
	obj->OnCollisionWith(coEvent);
}
