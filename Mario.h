#pragma once

#include <unordered_map>
#include "GameObject.h"
#include "BaseBrick.h"
#include "Character.h"
class CKoopa; // Forward declaration, stop circular dependency if include "Koopa.h"

#include "Animation.h"
#include "Animations.h"

#include "debug.h"
using namespace std;

enum class MarioLevel {
	SMALL,
	BIG,
	RACCOON
};

enum class MarioAnimationType {
	IDLE_RIGHT,
	IDLE_LEFT,
	WALKING_RIGHT,
	WALKING_LEFT,
	RUNNING_RIGHT,
	RUNNING_LEFT,
	JUMP_WALK_RIGHT,
	JUMP_WALK_LEFT,
	JUMP_RUN_RIGHT,
	JUMP_RUN_LEFT,
	SIT_RIGHT,
	SIT_LEFT,
	BRACE_RIGHT,
	BRACE_LEFT,
	KICK_RIGHT,
	KICK_LEFT,
	IDLE_HOLD_RIGHT,
	IDLE_HOLD_LEFT,
	WALK_HOLD_RIGHT,
	WALK_HOLD_LEFT,
	HOLD_FRONT,
	JUMP_HOLD_RIGHT,
	JUMP_HOLD_LEFT,
	TAIL_ATTACK_RIGHT,
	TAIL_ATTACK_LEFT,
	TAIL_JUMP_GLIDE_RIGHT,
	TAIL_JUMP_GLIDE_LEFT,
	TAIL_JUMP_FLY_RIGHT,
	TAIL_JUMP_FLY_LEFT,
	GROW_RIGHT,
	GROW_LEFT,
	SHRINK_RIGHT,
	SHRINK_LEFT,
	FLICKERING_RIGHT,
	FLICKERING_LEFT,
	DIE
};

namespace std {
	template <>
	struct hash<MarioLevel> {
		size_t operator()(const MarioLevel& k) const {
			return static_cast<int>(k);
		}
	};

	template <>
	struct hash<MarioAnimationType> {
		size_t operator()(const MarioAnimationType& k) const {
			return static_cast<int>(k);
		}
	};
}


#define MARIO_WALKING_SPEED		0.1f
#define MARIO_RUNNING_SPEED		0.24f
#define MARIO_SHELL_TURNING_SPEED		0.3f

#define MARIO_ACCEL_WALK_X	0.005625f
#define MARIO_ACCEL_RUN_X	0.0007f

#define MARIO_ACCEL_NORMAL_X	0.000196875f
#define MARIO_ACCEL_SKID_X	0.00045f
#define MARIO_SMALL_ACCEL_FRIC_X	0.000140625f
#define MARIO_BIG_ACCEL_FRIC_X	0.000196875f

#define MARIO_RUN_MAX_SPEED_X	0.15f
#define MARIO_RUN_P_MAX_SPEED_X	0.21f
#define MARIO_WALK_TOP_SPEED_X	0.09f

#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_JUMP_RUN_SPEED_Y	0.6f

#define MARIO_GRAVITY			0.002f
#define MARIO_GRAVITY_SLOW			0.000225f
#define MARIO_GRAVITY_FAST			0.001125f

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

#define GROUND_Y 160.0f


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

const int MARIO_JUMP_SPEED[4] = { 0.21, 0.2175, 0.225, 024 };

class CMario : public CCharacter
{
protected:
	static unordered_map<MarioLevel, std::unordered_map<MarioAnimationType, int>> animationMap;
	BOOLEAN isSitting;
	MarioLevel level;
	int untouchable;
	ULONGLONG untouchable_start;
	BOOLEAN isOnPlatform;
	int coin;
	CKoopa* holdingShell;
	int dirInput = 0; // 1: right, -1: left


	// timers for animations
	int attackTimer = 0; 
	int glideTimer = 0; 
	int flyTimer = 0; 

	int currentAnimation = -1;

	void OnCollisionWithCharacter(LPCOLLISIONEVENT e);
	void OnCollisionWithBaseBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithCoin(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);
	void OnCollisionWithPlant(LPCOLLISIONEVENT e);
	void OnCollisionWithFireball(LPCOLLISIONEVENT e);
	void OnCollisionWithMushroom(LPCOLLISIONEVENT e);
  void OnCollisionWithLeaf(LPCOLLISIONEVENT e);
	void GetAniId();
	void AssignCurrentAnimation(MarioLevel level, MarioAnimationType type) {
		//if not in list
		if (animationMap.find(level) == animationMap.end()) {
			DebugOut(L"[ERROR] Mario::AssignCurrentAnimation: level %d not found\n", level);
			return;
		}
		if (animationMap[level].find(type) == animationMap[level].end()) {
			DebugOut(L"[ERROR] Mario::AssignCurrentAnimation: type %d not found\n", type);
			return;
		}
		currentAnimation = animationMap[level][type];
		ResetCurrentAnimation();
	}

public:
	bool canHold = false;
	CMario(float x, float y) : CCharacter(x, y)
	{
		isSitting = false;
		maxVx = 0.0f;
		maxVy = 0.3f;
		ax = 0.0f;
		ay = MARIO_GRAVITY;
		level = MarioLevel::RACCOON;
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

	void Acceleration();

	int IsCollidable()
	{
		return (state != MARIO_STATE_DIE);
	}

	int IsBlocking() { return (state != MARIO_STATE_DIE && untouchable == 0); }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void SetLevel(MarioLevel l);
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

	void ResetCurrentAnimation() {
		if(currentAnimation > 0) {
			CAnimations::GetInstance()->Get(currentAnimation)->Reset();
		}
	
	}

	void TailAttackInit();
	void TailAttack(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void HoldTurn(int dir);
	void KickedShell();

	void SpecialPressed();
	void JumpPressed();

	MarioLevel GetLevel() { return level; }
	bool IsBig() { return level >= MarioLevel::BIG; }
};