#pragma once
#include "GameObject.h"
#include "Character.h"
class CKoopa; // Forward declaration, stop circular dependency if include "Koopa.h"

#include "Animation.h"
#include "Animations.h"

#include "debug.h"

#define MARIO_WALKING_SPEED		0.1f
#define MARIO_RUNNING_SPEED		0.2f
#define MARIO_SHELL_TURNING_SPEED		0.3f

#define MARIO_ACCEL_WALK_X	0.0005f
#define MARIO_ACCEL_RUN_X	0.0007f

#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_JUMP_RUN_SPEED_Y	0.6f

#define MARIO_GRAVITY			0.002f

#define MARIO_JUMP_DEFLECT_SPEED  0.4f

#define MARIO_STATE_DIE				-10
#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200

#define MARIO_STATE_JUMP			300
#define MARIO_STATE_RELEASE_JUMP    301

#define MARIO_STATE_RUNNING_RIGHT	400
#define MARIO_STATE_RUNNING_LEFT	500

#define MARIO_STATE_SIT				600
#define MARIO_STATE_SIT_RELEASE		601

#define MARIO_TURN_TIME 500


#pragma region ANIMATION_ID

#define ID_ANI_MARIO_IDLE_RIGHT 400
#define ID_ANI_MARIO_IDLE_LEFT 401

#define ID_ANI_MARIO_WALKING_RIGHT 500
#define ID_ANI_MARIO_WALKING_LEFT 501

#define ID_ANI_MARIO_RUNNING_RIGHT 600
#define ID_ANI_MARIO_RUNNING_LEFT 601

#define ID_ANI_MARIO_JUMP_WALK_RIGHT 700
#define ID_ANI_MARIO_JUMP_WALK_LEFT 701

#define ID_ANI_MARIO_JUMP_RUN_RIGHT 800
#define ID_ANI_MARIO_JUMP_RUN_LEFT 801

#define ID_ANI_MARIO_SIT_RIGHT 900
#define ID_ANI_MARIO_SIT_LEFT 901

#define ID_ANI_MARIO_BRACE_RIGHT 1000
#define ID_ANI_MARIO_BRACE_LEFT 1001

#define ID_ANI_MARIO_KICK_RIGHT	1010
#define ID_ANI_MARIO_KICK_LEFT	1011

#define ID_ANI_MARIO_DIE 999

// SMALL MARIO
#define ID_ANI_MARIO_SMALL_IDLE_RIGHT 1100
#define ID_ANI_MARIO_SMALL_IDLE_LEFT 1102

#define ID_ANI_MARIO_SMALL_WALKING_RIGHT 1200
#define ID_ANI_MARIO_SMALL_WALKING_LEFT 1201

#define ID_ANI_MARIO_SMALL_RUNNING_RIGHT 1300
#define ID_ANI_MARIO_SMALL_RUNNING_LEFT 1301

#define ID_ANI_MARIO_SMALL_BRACE_RIGHT 1400
#define ID_ANI_MARIO_SMALL_BRACE_LEFT 1401

#define ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT 1500
#define ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT 1501

#define ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT 1600
#define ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT 1601

#define ID_ANI_MARIO_SMALL_KICK_RIGHT 1610 
#define ID_ANI_MARIO_SMALL_KICK_LEFT 1611

// RACCOON MARIO
#define ID_ANI_MARIO_RACCOON_IDLE_RIGHT 1700
#define ID_ANI_MARIO_RACCOON_IDLE_LEFT 1702

#define ID_ANI_MARIO_RACCOON_WALKING_RIGHT 1800
#define ID_ANI_MARIO_RACCOON_WALKING_LEFT 1801

#define ID_ANI_MARIO_RACCOON_RUNNING_RIGHT 1900
#define ID_ANI_MARIO_RACCOON_RUNNING_LEFT 1901

#define ID_ANI_MARIO_RACCOON_BRACE_RIGHT 2000
#define ID_ANI_MARIO_RACCOON_BRACE_LEFT 2001

#define ID_ANI_MARIO_RACCOON_JUMP_WALK_RIGHT 2100
#define ID_ANI_MARIO_RACCOON_JUMP_WALK_LEFT 2101

#define ID_ANI_MARIO_RACCOON_JUMP_RUN_RIGHT 2200
#define ID_ANI_MARIO_RACCOON_JUMP_RUN_LEFT 2201

#define ID_ANI_MARIO_RACCOON_SIT_RIGHT 2300
#define ID_ANI_MARIO_RACCOON_SIT_LEFT 2301

#define ID_ANI_MARIO_RACCOON_TAIL_ATTACK_RIGHT 2400
#define ID_ANI_MARIO_RACCOON_TAIL_ATTACK_LEFT 2401

#define ID_ANI_MARIO_RACCOON_TAIL_JUMP_GLIDE_RIGHT 2500
#define ID_ANI_MARIO_RACCOON_TAIL_JUMP_GLIDE_LEFT 2501

#define ID_ANI_MARIO_RACCOON_TAIL_JUMP_FLY_RIGHT 2600
#define ID_ANI_MARIO_RACCOON_TAIL_JUMP_FLY_LEFT 2601

#define ID_ANI_MARIO_RACCOON_KICK_RIGHT	2610
#define ID_ANI_MARIO_RACCOON_KICK_LEFT	2611

#pragma endregion

#define GROUND_Y 160.0f




#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define	MARIO_LEVEL_RACCOON		3

#define MARIO_BIG_BBOX_WIDTH  14
#define MARIO_BIG_BBOX_HEIGHT 24
#define MARIO_BIG_SITTING_BBOX_WIDTH  14
#define MARIO_BIG_SITTING_BBOX_HEIGHT 16

#define MARIO_SIT_HEIGHT_ADJUST ((MARIO_BIG_BBOX_HEIGHT-MARIO_BIG_SITTING_BBOX_HEIGHT)/2)

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 12

// time = animation time (sum of all frame duration)
#define ATTACK_TIME	400
#define GLIDE_TIME	300
#define FLY_TIME	300

#define MARIO_UNTOUCHABLE_TIME 2500

class CMario : public CCharacter
{
protected:
	BOOLEAN isSitting;
	int level;
	int untouchable;
	ULONGLONG untouchable_start;
	BOOLEAN isOnPlatform;
	int coin;
	CKoopa* holdingShell;

	// timers for animations
	int attackTimer = 0; 
	int glideTimer = 0; 
	int flyTimer = 0; 

	int currentAnimation = -1;

	void OnCollisionWithCharacter(LPCOLLISIONEVENT e);
	void OnCollisionWithCoin(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);
	void OnCollisionWithPlant(LPCOLLISIONEVENT e);
	void OnCollisionWithFireball(LPCOLLISIONEVENT e);
	void OnCollisionWithQuestionBlock(LPCOLLISIONEVENT e);
	void OnCollisionWithMushroom(LPCOLLISIONEVENT e);
	int GetAniIdBig();
	int GetAniIdSmall();
	int GetAniIdRaccoon();

public:
	bool canHold = false;
	CMario(float x, float y) : CCharacter(x, y)
	{
		isSitting = false;
		maxVx = 0.0f;
		maxVy = 0.3f;
		ax = 0.0f;
		ay = MARIO_GRAVITY;

		level = MARIO_LEVEL_RACCOON;
		untouchable = 0;
		untouchable_start = -1;
		isOnPlatform = false;
		coin = 0;
		canHold = false;

		holdingShell = NULL;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);

	int IsCollidable()
	{
		return (state != MARIO_STATE_DIE);
	}

	int IsBlocking() { return (state != MARIO_STATE_DIE && untouchable == 0); }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void SetLevel(int l);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void GetTailHitBox(float& l1, float& t1, float& r1, float& b1, float& l2, float& t2, float& r2, float& b2);
	void SetCanHold(bool pick) { this->canHold = pick; }
	void Drop() { holdingShell = NULL; }
	void HoldingProcess(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Attacked();
	virtual void Stomped() {};
	virtual void ShellHit(int shellX) {};
	virtual void TailHit() {};
	virtual void BlockHit() {};

	void TailAttackInit();
	void TailAttack(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void KickedShell();

	void SpecialPressed();
	void JumpPressed();

	int GetLevel() { return level; }
};