#pragma once

#include <Windows.h>
#include <d3dx10.h>
#include <vector>

#include "Animation.h"
#include "Animations.h"
#include "Sprites.h"
#include "Collision.h"

#define TILE_SIZE 16

using namespace std;

#define ID_TEX_BBOX -100		// special texture to draw object bounding box
#define BBOX_ALPHA 0.25f		// Bounding box transparency

class CGameObject
{
protected:

	float ox = 0, oy = 0; // old x, y before entering blocking zone

	float x; 
	float y;

	float vx = 0;
	float vy = 0;

	int nx;	 

	int state;

	bool isDeleted = false; 

	bool sleep = false; 

	bool killOffCam = false;

public: 
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetPositionX(float x) { this->x = x; }
	void SetY(float y) { this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void SetSleep(bool sleep) { this->sleep = sleep; }
	bool GetKillOffCam() { return killOffCam; }

	void GetPosition(float &x, float &y) { x = this->x; y = this->y; }
	int GetX() { return x; }
	int GetY() { return y; }
	void GetSpeed(float &vx, float &vy) { vx = this->vx; vy = this->vy; }
	bool GetSleep() { return sleep; }

	int GetState() { return this->state; }
	virtual void Delete() { isDeleted = true;  }
	bool IsDeleted() { return isDeleted; }

	void RenderBoundingBox();

	CGameObject();
	CGameObject(float x, float y) :CGameObject() { this->x = x; this->y = y; }


	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {
		left = top = right = bottom = 0; // default bounding box is (0,0,0,0)
	};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {};
	virtual void UpdateLate() {};
	virtual void Render() = 0;
	virtual void SetState(int state) { this->state = state; }

	//
	// Collision ON or OFF ? This can change depending on object's state. For example: die
	//
	virtual int IsCollidable() { return 0; };

	void GetCollidableObjects(vector<LPGAMEOBJECT>* coObjects);

	// When no collision has been detected (triggered by CCollision::Process)
	virtual void OnNoCollision(DWORD dt) {};

	// When collision with an object has been detected (triggered by CCollision::Process)
	virtual void OnCollisionWith(LPCOLLISIONEVENT e) {};
	
	// Is this object blocking other object? If YES, collision framework will automatically push the other object
	virtual int IsBlocking() { return 1; }

	// Does this object collide with other object at certain direction ( like ColorBox )
	virtual int IsDirectionColliable(float nx, float ny) { return 1; }

	void Clear();

	~CGameObject();

	static bool IsDeleted(const LPGAMEOBJECT &o) { return o->isDeleted; }
	
	bool GetIsPause();
	bool GetIsStop();

	void SetOldPosition(float x, float y) {
		ox = x;
		oy = y;
	};

	void GetOldPosition(float &x, float &y) {
		x = ox;
		y = oy;
	};

};
