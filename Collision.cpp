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
#include "Leaf.h"
#include "ScoreManager.h"
#include "QuestionBlock.h"
#include "Koopa.h"
#include "Utils.h"
#include "Floor.h"
#include "Box.h"
#include "Pipe.h"
#include "InvisibleWall.h"
#include "Plant.h"
#include <unordered_set>
#define BLOCK_PUSH_FACTOR 0.01f

#define DIRECTION_LEFT	0
#define DIRECTION_TOP	1
#define DIRECTION_RIGHT	2
#define DIRECTION_BOTTOM	3

CCollision* CCollision::__instance = NULL;

static bool IsOverlapping(float al, float at, float ar, float ab, float bl, float bt, float br, float bb) {
	return al < br && ar > bl && at < bb && ab > bt;
}

static bool IsOverlapping(LPGAMEOBJECT a, LPGAMEOBJECT b, DWORD dt) {
	float al, at, ar, ab;
	a->GetBoundingBox(al, at, ar, ab);
	float bl, bt, br, bb;
	b->GetBoundingBox(bl, bt, br, bb);
	float avx, avy;
	a->GetSpeed(avx, avy);
	float adx = avx * dt;
	float ady = avy * dt;
	float bvx, bvy;
	b->GetSpeed(bvx, bvy);
	float bdx = bvx * dt;
	float bdy = bvy * dt;
	return IsOverlapping(al + adx, at + ady, ar + adx, ab + ady, bl + bdx, bt + bdy, br + bdx, bb + bdy);

}

// Check right now (no vx vy applied) did collision happened
static bool IsOverlappingNow(LPGAMEOBJECT a, LPGAMEOBJECT b, DWORD dt) {
	float al, at, ar, ab;
	a->GetBoundingBox(al, at, ar, ab);
	float bl, bt, br, bb;
	b->GetBoundingBox(bl, bt, br, bb);
	return IsOverlapping(al, at, ar, ab, bl, bt, br, bb);
}

static int StillOverlapping(float al, float at, float ar, float ab, float dx, float dy, float bl, float bt, float br, float bb) {
	int curTouch = IsOverlapping(al, at, ar, ab, bl, bt, br, bb);
	int nexTouch = IsOverlapping(al + dx, at + dy, ar + dx, ab + dy, bl, bt, br, bb);
	if (curTouch == 0) return 0; // not touch
	return  curTouch + nexTouch; // 2: still touch, 1: touch then not touch
}

LPCOLLISIONEVENT CCollisionEvent::Reverse(LPCOLLISIONEVENT e)
{
	LPCOLLISIONEVENT ne = new CCollisionEvent(e->t, -e->nx, -e->ny, -e->dx, -e->dy, e->src_obj, e->obj);
	ne->src_obj->OnCollisionWith(ne);
	return ne;

}

int CCollisionEvent::WasCollided() {
	return
		t >= 0.0f && t <= 1.0f && obj->IsDirectionColliable(nx, ny) == 1;
}

bool CCollisionEvent::PrioritizeBlockingEvent(const LPCOLLISIONEVENT a, const LPCOLLISIONEVENT b)
{
	if (a->obj->IsBlocking() && !b->obj->IsBlocking()) return true;
	if (!a->obj->IsBlocking() && b->obj->IsBlocking()) return false;
	return a->t < b->t;
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

	t_entry = std::max<float>(tx_entry, ty_entry);
	t_exit = std::min<float>(tx_exit, ty_exit);

	if (t_entry > t_exit) return;

	t = t_entry;


	//ex: if going right, nx is negative 
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
	// ignore collision with box
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

//type 1: only blocking, type 2: only non blocking
// dir 0, 1, 2, 3: left top right bottom (the moving direction of objsrc)
void CCollision::Scan(LPGAMEOBJECT objSrc, DWORD dt, vector<LPGAMEOBJECT>* objDests, vector<LPCOLLISIONEVENT>& coEvents, int type, int dir)
{
	for (auto obj : *objDests)
	{
		if (!obj->IsCollidable()) continue; // if the other obj not collidable then skip (2 way)
		if(obj->IsBoundBoxZero()) continue; // if the other obj not collidable then skip (2 way)	
		if(dynamic_cast<CMario*>(obj) && dynamic_cast<CBox*>(objSrc)) continue; 
		if (dynamic_cast<CMovingPlatform*>(obj) && dynamic_cast<CKoopa*>(objSrc)) continue; // skip koopa on moving platform
		if (dynamic_cast<CBaseBrick*>(obj) && dynamic_cast<CMovingPlatform*>(objSrc)) continue; //skip movingplatform on basebrick
		if (dynamic_cast<CBox*>(objSrc) == NULL && dynamic_cast<CInvisibleWall*>(obj)) continue; //skip invisible wall
		

		if (type == 1 && !obj->IsBlocking()) continue;
		else if (type == 2 && obj->IsBlocking()) continue;
		

		LPCOLLISIONEVENT e = SweptAABB(objSrc, dt, obj);

		if (dynamic_cast<CKoopa*>(objSrc) && dynamic_cast<CFloor*>(obj)) {
			CFloor* floor = dynamic_cast<CFloor*>(obj);
			if (floor->GetType() == 1 && e->nx != 0) {
				delete e;
				continue; // koopa can move through floor(type==1)
			}
		}

		if (dir != -1) {
			if (!(dir == 0 && e->nx < 0  // left
				|| dir == 1 && e->ny < 0  // top
				|| dir == 2 && e->nx > 0  // right
				|| dir == 3 && e->ny > 0)) // bottom
			{
				delete e;
				continue;
			}
		}

		if (e->WasCollided() == 1) {
			coEvents.push_back(e);
		}
		else {
			delete e;
		}
	}
}

void CCollision::Filter(LPGAMEOBJECT objSrc,
	vector<LPCOLLISIONEVENT>& coEvents,
	LPCOLLISIONEVENT& colX,
	LPCOLLISIONEVENT& colY,
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
void CCollision::ProcessCollision(LPGAMEOBJECT objSrc, DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float x, y;
	objSrc->GetPosition(x, y);
	vector<LPCOLLISIONEVENT> coEvents;
	LPCOLLISIONEVENT colX = NULL;
	LPCOLLISIONEVENT colY = NULL;

	coEvents.clear();

	if (!objSrc->IsCollidable() || objSrc->IsBoundBoxZero()) {
		objSrc->OnNoCollision(dt);
		return;
	}

	Scan(objSrc, dt, coObjects, coEvents, 1, -1);

	int eventCount = coEvents.size();

	// No collision detected
	if (coEvents.size() > 0)
	{
		Filter(objSrc, coEvents, colX, colY);

		float vx, vy, dx, dy;
		objSrc->GetSpeed(vx, vy);
		dx = vx * dt;
		dy = vy * dt;
		if (colX != NULL && colY != NULL)
		{
			if (colY->t < colX->t)	// was collision on Y first ?
			{
				y += colY->t * dy + colY->ny * BLOCK_PUSH_FACTOR;
				objSrc->OnCollisionWith(colY); // set position and call OnCollisionWith

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
					x += colX_other->t * dx + colX_other->nx * BLOCK_PUSH_FACTOR;
					objSrc->OnCollisionWith(colX_other);
				}
				else
				{
					x += dx;
				}
			}
			else // collision on X first
			{
				x += colX->t * dx + colX->nx * BLOCK_PUSH_FACTOR;
				objSrc->OnCollisionWith(colX);
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
					objSrc->OnCollisionWith(colY_other);
				}
				else
				{
					y += dy;
				}
			}
		}
		else
			if (colX != NULL)
			{
				x += colX->t * dx + colX->nx * BLOCK_PUSH_FACTOR;
				y += dy;
				objSrc->OnCollisionWith(colX);
			}
			else
				if (colY != NULL)
				{
					x += dx;
					y += colY->t * dy + colY->ny * BLOCK_PUSH_FACTOR;
					objSrc->OnCollisionWith(colY);
				}
				else // both colX & colY are NULL 
				{
					x += dx;
					y += dy;
				}
	}
	objSrc->SetPosition(x, y);

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	coEvents.clear();

	// process non-blocking collisions
	Scan(objSrc, dt, coObjects, coEvents, 2, -1);

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT e = coEvents[i];
		if (e->isDeleted) continue;
		if (e->obj->IsBlocking()) continue;  // blocking collisions were handled already, skip them
		objSrc->OnCollisionWith(e);
	}


	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	if (eventCount == 0) objSrc->OnNoCollision(dt);
}

void CCollision::ProcessNoBlock(LPGAMEOBJECT objSrc, DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float x, y;
	objSrc->GetPosition(x, y);
	vector<LPCOLLISIONEVENT> coEvents;
	LPCOLLISIONEVENT colX = NULL;
	LPCOLLISIONEVENT colY = NULL;

	coEvents.clear();

	if (!objSrc->IsCollidable() || objSrc->IsBoundBoxZero()) {
		objSrc->OnNoCollision(dt);
		return;
	}

	

	// process non-blocking collisions
	Scan(objSrc, dt, coObjects, coEvents, 2, -1);

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT e = coEvents[i];
		if (e->isDeleted) continue;
		if (e->obj->IsBlocking()) continue;  // blocking collisions were handled already, skip them
		objSrc->OnCollisionWith(e);
	}


	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	objSrc->OnNoCollision(dt);
}

void CCollision::ProcessOverlap(LPGAMEOBJECT objSrc, DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vector<LPCOLLISIONEVENT> coEvents;
	// all object have sprite box is included in this function
	float ml, mt, mr, mb;
	objSrc->GetSpriteBox(ml, mt, mr, mb);
	float vx, vy;
	objSrc->GetSpeed(vx, vy);
	float mdx = vx * dt;
	float mdy = vy * dt;
	for (auto i : *coObjects) {
		if (objSrc == i) continue;
		float sl, st, sr, sb;
		i->GetSpriteBox(sl, st, sr, sb);
		float svx, svy;
		i->GetSpeed(svx, svy);
		float sdx = svx * dt;
		float sdy = svy * dt;
		if (IsOverlapping(ml + mdx, mt + mdy, mr + mdx, mb + mdy, sl + sdx, st + sdy, sr + sdx, sb + sdy)) {
			LPCOLLISIONEVENT e = new CCollisionEvent(0, 0, 0, mdx, mdy, i, objSrc);
			coEvents.push_back(e);
			objSrc->OnOverlapWith(e);
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	coEvents.clear();
}

void CCollision::ProcessOverlapNow(LPGAMEOBJECT objSrc, DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vector<LPCOLLISIONEVENT> coEvents;
	for (auto i : *coObjects) {
		if (objSrc == i) continue;
		if (IsOverlappingNow(objSrc, i, dt)) {
			LPCOLLISIONEVENT e = new CCollisionEvent(0, 0, 0, 0, 0, i, objSrc);
			coEvents.push_back(e);
			objSrc->OnOverlapWith(e);
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	coEvents.clear();
}

void CCollision::ProcessMarioPoints(LPGAMEOBJECT objSrc, vector<CPoint*>* points, vector<LPGAMEOBJECT>* coObjects, DWORD dt)
{
	if (!objSrc->IsCollidable()) {
		objSrc->OnNoCollision(dt);
		return;
	}
	//collision check
	float x, y;
	float vx, vy;

	objSrc->GetPosition(x, y);
	objSrc->GetSpeed(vx, vy);
	unordered_set<LPGAMEOBJECT> marked;

	vector<LPCOLLISIONEVENT> coEventsVertical;
	vector<LPCOLLISIONEVENT> coEventsHorizontal;
	vector<LPCOLLISIONEVENT> coEvents;
	LPCOLLISIONEVENT colX = NULL;
	LPCOLLISIONEVENT colY = NULL;

	coEvents.clear();
	coEventsVertical.clear();
	coEventsHorizontal.clear();

	// collision objects for mario (head, body, foot);

	float head_l, head_t, head_r, head_b;
	float body_l, body_t, body_r, body_b;
	float foot_l, foot_t, foot_r, foot_b;
	float mario_l, mario_t, mario_r, mario_b;
	objSrc->GetBoundingBox(mario_l, mario_t, mario_r, mario_b);

	//convert mario 7 points to 3 boxes for collision check

	(*points)[TOP]->GetBoundingBox(head_l, head_t, head_r, head_b);

	body_l = (*points)[LEFTUP]->GetBoundingBoxLeft();
	body_t = (*points)[RIGHTUP]->GetBoundingBoxTop();
	body_r = (*points)[RIGHTDOWN]->GetBoundingBoxRight();
	body_b = (*points)[LEFTDOWN]->GetBoundingBoxBottom();

	foot_l = (*points)[DOWNLEFT]->GetBoundingBoxLeft();
	foot_t = (*points)[DOWNLEFT]->GetBoundingBoxTop();
	foot_r = (*points)[DOWNRIGHT]->GetBoundingBoxRight();
	foot_b = (*points)[DOWNRIGHT]->GetBoundingBoxBottom();

	CBox* head = new CBox(head_l, head_t, head_r, head_b);
	head->SetSpeed(vx, vy);
	head->SetPosition(x, y);

	CBox* body = new CBox(body_l, body_t, body_r, body_b);
	body->SetSpeed(vx, vy);
	body->SetPosition(x, y);

	CBox* foot = new CBox(foot_l, foot_t, foot_r, foot_b);	
	foot->SetSpeed(vx, vy);
	foot->SetPosition(x, y);

	Scan(head, dt, coObjects, coEventsVertical, 1, 3);
	Scan(body, dt, coObjects, coEventsHorizontal, 1, 0);
	Scan(body, dt, coObjects, coEventsHorizontal, 1, 2);
	Scan(foot, dt, coObjects, coEventsVertical, 1, 1);

	for(auto i: coEventsVertical)
		coEvents.push_back(i);
	for (auto i : coEventsHorizontal)
		coEvents.push_back(i);
	

	int eventCount = coEvents.size();

	if (coEvents.size() > 0)
	{
		//objsrc doesnt affect filter
		Filter(objSrc, coEventsVertical, colX, colY, 1, 0, 1);
		Filter(objSrc, coEventsHorizontal, colX, colY, 1, 1, 0);

		float vx, vy, dx, dy;
		objSrc->GetSpeed(vx, vy);
		dx = vx * dt;
		dy = vy * dt;
		if (colX != NULL && colY != NULL)
		{
			if (colY->t < colX->t)	// was collision on Y first ?
			{
				y += colY->t * dy + colY->ny * BLOCK_PUSH_FACTOR;
				objSrc->OnCollisionWith(colY); // set position and call OnCollisionWith
				//marked.insert(colY->obj);
				//
				// see if after correction on Y, is there still a collision on X ? 
				//
				LPCOLLISIONEVENT colX_other = NULL;

				//
				// check again if there is true collision on X 
				//
				colX->isDeleted = true;		// remove current collision event on X

				// replace with a new collision event using corrected location 
				coEvents.push_back(SweptAABB(body, dt, colX->obj));

				// re-filter on X only
				Filter(objSrc, coEvents, colX_other, colY, /*filterBlock = */ 1, 1, /*filterY=*/0);

				if (colX_other != NULL)
				{
					x += colX_other->t * dx + colX_other->nx * BLOCK_PUSH_FACTOR;
					objSrc->OnCollisionWith(colX_other);
					//marked.insert(colX_other->obj);
				}
				else
				{
					x += dx;
				}
			}
			else // collision on X first
			{
				x += colX->t * dx + colX->nx * BLOCK_PUSH_FACTOR;
				objSrc->OnCollisionWith(colX);
				//marked.insert(colX->obj);

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
				Filter(head, coEvents, colX, colY_other, /*filterBlock = */ 1, /*filterX=*/0, /*filterY=*/1);
				Filter(foot, coEvents, colX, colY_other, /*filterBlock = */ 1, /*filterX=*/0, /*filterY=*/1);

				if (colY_other != NULL)
				{
					y += colY_other->t * dy + colY_other->ny * BLOCK_PUSH_FACTOR;
					objSrc->OnCollisionWith(colY_other);
					//marked.insert(colY_other->obj);
				}
				else
				{
					y += dy;
				}
			}
		}
		else
			if (colX != NULL)
			{
				x += colX->t * dx + colX->nx * BLOCK_PUSH_FACTOR;
				y += dy;
				objSrc->OnCollisionWith(colX);
				//marked.insert(colX->obj);
			}
			else
				if (colY != NULL)
				{
					x += dx;
					y += colY->t * dy + colY->ny * BLOCK_PUSH_FACTOR;
					objSrc->OnCollisionWith(colY);
					//marked.insert(colY->obj);
				}
				else // both colX & colY are NULL 
				{
					x += dx;
					y += dy;
				}
		}
	objSrc->SetPosition(x, y);

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	coEvents.clear();
	coEventsVertical.clear();
	coEventsHorizontal.clear();

	// process non-blocking collisions
	Scan(head, dt, coObjects, coEventsVertical, 0, 3);
	Scan(body, dt, coObjects, coEventsHorizontal, 0, 0);
	Scan(body, dt, coObjects, coEventsHorizontal, 0, 2);
	Scan(foot, dt, coObjects, coEventsVertical, 0, 1);

	for (auto i : coEventsVertical)
		coEvents.push_back(i);
	for (auto i : coEventsHorizontal)
		coEvents.push_back(i);
	//eventCount += coEvents.size();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT e = coEvents[i];
		if (marked.find(e->obj) != marked.end()) continue;
		if (e->isDeleted) continue;
		if (dynamic_cast<CCharacter*>(e->obj) && e->ny != -1) continue; //skip collision with enemies when not stomping, overlap for hit
		//if (e->obj->IsBlocking()) continue;  // blocking collisions were handled already, skip them
		objSrc->OnCollisionWith(e);
		marked.insert(e->obj);
	}


	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	coEvents.clear();
	coEventsVertical.clear();
	coEventsHorizontal.clear();

	delete head;
	delete body;
	delete foot;

	if (eventCount == 0) objSrc->OnNoCollision(dt);
	if (((CMario*)(objSrc))->GetPointsDisable() > 0) return;

	vector<bool> pointsTouched;
	vector<bool> pointsMaybeTouched;
	vector<LPGAMEOBJECT>  collidedObjects;
	pointsTouched.clear();
	pointsMaybeTouched.clear();
	collidedObjects.clear();

	CMario* mario = dynamic_cast<CMario*>(objSrc);

	//overlap with block
	#pragma region overlap_block
	
	objSrc->GetPosition(x, y);
	objSrc->GetSpeed(vx, vy);

	int k = 0;
	float idk;
	float footBeforeY, footBeforeX;
	float bodyLeftBeforeX, bodyRightBeforeX;
	
	
	mario->SetPointsPosition();

	(*points)[DOWNLEFT]->GetBeforeBlockPosition(idk, footBeforeY);
	(*points)[RIGHTUP]->GetBeforeBlockPosition(bodyRightBeforeX, idk);
	(*points)[LEFTUP]->GetBeforeBlockPosition(bodyLeftBeforeX, idk);

	bool overlapedInvisibleWall = false;
	bool overlapedBaseBrick = false;
	for (auto i : *points) {
		CPoint* point = i;
		bool touched = false;
		bool maybe = false;

		for (auto obj : *coObjects)
		{
			if(dynamic_cast<CMario*>(obj)) continue; //skip brick objects

			if (obj->IsBlocking())
			{
				float sl, st, sr, sb;
				obj->GetBoundingBox(sl, st, sr, sb);
				float ml, mt, mr, mb;
				point->GetBoundingBox(ml, mt, mr, mb);

				if (IsOverlapping(ml, mt, mr, mb, sl, st, sr, sb))
				{
					pointsMaybeTouched.push_back(true);
					maybe = true;
				}

				if (dynamic_cast<CGenericPlatform*>(obj)) {
					if (k == RIGHTUP || k == RIGHTDOWN) {
						if (vx > 0 || footBeforeY >= st) continue; // skip if moving up to platform
					}
					else if (k == LEFTUP || k == LEFTDOWN) {
						if (vx < 0 || footBeforeY >= st) continue;
					}
				}

				if (IsOverlapping(ml, mt, mr, mb, sl, st, sr, sb))
				{
					if (dynamic_cast<CInvisibleWall*>(obj)) {
						overlapedInvisibleWall = true;
					}
					if (dynamic_cast<CBaseBrick*>(obj)) {
						overlapedBaseBrick = true;
					}
					collidedObjects.push_back(obj);
					pointsTouched.push_back(true);
					touched = true;
					break;
				}
			}
			//mario stomp enemy even when the feet collided sideways
			//else if (dynamic_cast<CCharacter*>(obj)) {}
		}
		if (!touched) {
			pointsTouched.push_back(false);
			i->SetBeforeBlockPosition(i->GetX(), i->GetY());
			collidedObjects.push_back(NULL);
		}

		if(!maybe) {
			pointsMaybeTouched.push_back(false);
		}

		k++;
	}

	if(overlapedBaseBrick && overlapedInvisibleWall) {
		objSrc->SetState(MARIO_STATE_DIE);
		return;
	}

	int dirX = 0;
	int dirY = 0;

	const float X_PUSH_SPEED = 0.08f;
	const float Y_PUSH_SPEED = 0.12f;

	// not a good idea to change directly by frame, should use dt to account for the lag
	if (pointsTouched[LEFTUP] || pointsTouched[LEFTDOWN]) {
		dirX = X_PUSH_SPEED * dt;
	}
	else if (pointsTouched[RIGHTUP] || pointsTouched[RIGHTDOWN]) {
		dirX = -X_PUSH_SPEED * dt;
	}

	if ((pointsTouched[DOWNLEFT] || pointsTouched[DOWNRIGHT]) && !(pointsMaybeTouched[LEFTDOWN] || pointsMaybeTouched[RIGHTDOWN])) {
		dirY = -Y_PUSH_SPEED * dt;
	}

	x += dirX;
	y += dirY;
	if ((vx < 0 && dirX > 0) || (vx > 0 && dirX < 0)) {
		vx = 0;
	}

	if ((vy < 0 && dirY > 0) || (vy > 0 && dirY < 0)) {
		vy = 0;
	}
	objSrc->SetPosition(x, y);
	objSrc->SetSpeed(vx, vy);

	pointsTouched.clear();
	pointsMaybeTouched.clear();
	collidedObjects.clear();

	#pragma endregion

	#pragma region overlap_non_block
	objSrc->GetPosition(x, y);
	objSrc->GetSpeed(vx, vy);
	k = 0;
	mario->SetPointsPositionForNonBlockingOverlap();

	for (auto obj : *coObjects)
	{
		if (dynamic_cast<CMario*>(obj)) continue; //skip brick objects
		if(mario->holdingShell!=NULL && mario->holdingShell == dynamic_cast<CKoopa*>(obj)) continue; //skip holding shell
		if (marked.find(obj) != marked.end()) continue;

		if (obj->IsBlocking()) continue;
			
		for (auto i : *points) pointsTouched.push_back(IsOverlappingNow(i, obj, dt));
		for (auto i : *points) pointsMaybeTouched.push_back(IsOverlappingNow(i, obj, dt));

		LPCOLLISIONEVENT e = NULL;

		if(CCharacter* character = dynamic_cast<CCharacter*>(obj)) {
			if (pointsTouched[TOP] || pointsTouched[LEFTUP] || pointsTouched[LEFTDOWN] || pointsTouched[RIGHTUP] || pointsTouched[RIGHTDOWN]) {
				int nx = 0, ny = 0;
				if (pointsTouched[RIGHTUP] || pointsTouched[RIGHTDOWN]) nx = -1;
				else if (pointsTouched[LEFTUP] || pointsTouched[LEFTDOWN]) nx = 1;
				else ny = 1;
				e = new CCollisionEvent(0, nx, ny, vx, vy, obj, objSrc);
			}
			//else if (((pointsTouched[DOWNLEFT] || pointsTouched[DOWNRIGHT])) && !(pointsMaybeTouched[LEFTDOWN] || pointsMaybeTouched[RIGHTDOWN]) || pointsMaybeTouched[LEFTUP] || pointsMaybeTouched[RIGHTUP])
			//	e = new CCollisionEvent(0, 0, -1, vx, vy, obj, objSrc);
		}
		// not enemy
		else if (pointsTouched[TOP] || pointsTouched[LEFTUP] || pointsTouched[LEFTDOWN] || pointsTouched[RIGHTUP] || pointsTouched[RIGHTDOWN] || pointsTouched[DOWNLEFT] || pointsTouched[DOWNRIGHT])
			e = new CCollisionEvent(0, 0, 0, vx, vy, obj, objSrc);
		
		if (e != NULL) {
			objSrc->OnCollisionWith(e);
			marked.insert(e->obj);
			delete e;
		}
		pointsTouched.clear();
		pointsMaybeTouched.clear();
	}
		
	#pragma endregion
	marked.clear();
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

				mxOutCome = max(mxOutCome, StillOverlapping(ml, mt, mr, mb, dx, dy, sl, st, sr, sb));
			}
		}
	}return mxOutCome;
}

bool CCollision::CheckTouchingSolid(LPGAMEOBJECT obj, DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float ml, mt, mr, mb;
	obj->GetBoundingBox(ml, mt, mr, mb);
	int mxOutCome = 0;
	if (coObjects->size() > 0)
	{
		for (auto obj : *coObjects)
		{
			if (dynamic_cast<CBaseBrick*>(obj) || dynamic_cast<CPipe*>(obj))
			{
				float sl, st, sr, sb;
				obj->GetBoundingBox(sl, st, sr, sb);
				//
				// NOTE: new m speed = original m speed - collide object speed
				// 
				if (IsOverlapping(ml, mt, mr, mb, sl, st, sr, sb)) return true;
			}
		}
	}return false;
}

bool CCollision::CheckTouchCharacterForShellHeldHit(LPGAMEOBJECT objSrc, DWORD dt, vector<LPGAMEOBJECT>* coObjects, bool notMario = 1)
{
	float ml, mt, mr, mb;
	objSrc->GetBoundingBox(ml, mt, mr, mb);
	float vx, vy;
	objSrc->GetSpeed(vx, vy);
	bool isTouching = false;
	if (coObjects->size() > 0)
	{
		for (auto obj : *coObjects)
		{
			if (notMario && dynamic_cast<CMario*>(obj)) continue;
			if (dynamic_cast<CCharacter*>(obj) == NULL) continue;
			if (obj == objSrc) continue;
			CCharacter* character = dynamic_cast<CCharacter*>(obj);
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

				if (IsOverlapping(ml + dx, mt + dy, mr + dx, mb + dy, sl, st, sr, sb)) {
					isTouching = true;
					DebugObjectType(obj);
					if (CCharacter* character = dynamic_cast<CCharacter*>(obj)) {
						character->ShellHeldHit((ml + mr) / 2);
					}
				}
			}
		}
	}return isTouching;
}

bool CCollision::CheckTouchCharacterForTailAttack(float ml, float mt, float mr, float mb, float vx, float vy, DWORD dt, vector<LPGAMEOBJECT>* coObjects, float x, int nx, float y, bool& brickHit)
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

				if (IsOverlapping(ml + dx, mt + dy, mr + dx, mb + dy, sl, st, sr, sb)) {
					isTouching = true;
					CPlant* plant = dynamic_cast<CPlant*>(obj);
					if (CCharacter* character = dynamic_cast<CCharacter*>(obj))
					{
						character->TailHit(x);

						if (!dynamic_cast<CPlant*>(obj)) CGameFXManager::GetInstance()->AddGameFX((ml + mr) / 2, (mt + mb) / 2, 1);
						else if (plant->ReturnTailEffect()) CGameFXManager::GetInstance()->AddGameFX((ml + mr) / 2, (mt + mb) / 2, 1);

						if (!dynamic_cast<CKoopa*>(obj) && !dynamic_cast<CPlant*>(obj))
						{
							float enemy_x, enemy_y;
							obj->GetPosition(enemy_x, enemy_y);
							CScoreManager::GetInstance()->AddScore(enemy_x, enemy_y, SCORE_100);
						}
					}
					else if (CBaseBrick* brick = dynamic_cast<CBaseBrick*>(obj))
					{
						if (nx > 0 && !brickHit)
						{
							brickHit = true;
							brick->SideHit();
							return isTouching;
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