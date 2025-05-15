#pragma once

#include <unordered_map>
#include "GameObject.h"
#include "BaseBrick.h"
#include "Character.h"
#include "Timer.h"
#include "Point.h"
class CKoopa; // Forward declaration, stop circular dependency if include "Koopa.h"

#include "Animation.h"
#include "Animations.h"

#include "debug.h"
using namespace std;

// point ids
#define TOP 0
#define RIGHTUP 1
#define RIGHTDOWN 2
#define DOWNRIGHT 3
#define DOWNLEFT 4
#define LEFTDOWN 5
#define LEFTUP 6

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
	SPRINTING_RIGHT,
	SPRINTING_LEFT,
	JUMP_RIGHT,
	JUMP_LEFT,
	JUMP_FALL_RIGHT,
	JUMP_FALL_LEFT,
	JUMP_SPRINT_RIGHT,
	JUMP_SPRINT_LEFT,
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
	RUN_HOLD_RIGHT,
	RUN_HOLD_LEFT,
	SPRINT_HOLD_RIGHT,
	SPRINT_HOLD_LEFT,
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

#define FRAME_RATE (1000/60)

#define MARIO_WALKING_SPEED		0.1f
#define MARIO_RUNNING_SPEED		0.24f
#define MARIO_SHELL_TURNING_SPEED		0.3f

#define MARIO_ACCEL_WALK_X	0.005625f
#define MARIO_ACCEL_RUN_X	0.0007f

#define MARIO_ACCEL_NORMAL_X	0.000196875f
#define MARIO_ACCEL_SKID_X	0.00045f
#define MARIO_SMALL_ACCEL_FRIC_X	0.000140625f
#define MARIO_BIG_ACCEL_FRIC_X	0.000196875f

#define MARIO_ACCEL_MIDAIR_X	0.00021094f
#define MARIO_DECEL_MIDAIR_X	0.00045f

#define MARIO_RACCOON_MIDAIR_SPEED_LIMIT 0.08625f
#define MARIO_RACCOON_MIDAIR_DECEL 0.000225f

#define MARIO_RUN_MAX_SPEED_X	0.15f
#define MARIO_SPRINT_MAX_SPEED_X	0.21f
#define MARIO_WALK_MAX_SPEED_X	0.09f

#define MARIO_JUMP_SPEED_Y		0.20625f
#define MARIO_JUMP_RUN_SPEED_Y	0.225f

#define MARIO_GRAVITY			0.002f
#define MARIO_GRAVITY_SLOW			0.000225f
#define MARIO_GRAVITY_FAST			0.001125f
#define MARIO_MAX_FALL_SPEED_Y			0.24f

#define MARIO_JUMP_DEFLECT_SPEED  0.24f
#define MARIO_JUMP_WEAK_DEFLECT_SPEED  0.18f

#define MARIO_RACCOON_FLY_SPEED  0.09f
#define MARIO_RACCOON_GLIDE_SPEED  0.06f

#define MARIO_STATE_DIE				-1
#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200

#define MARIO_STATE_JUMP			300
#define MARIO_STATE_RELEASE_JUMP    301

#define MARIO_STATE_RUNNING_RIGHT	400
#define MARIO_STATE_RUNNING_LEFT	500

#define MARIO_STATE_SIT				600
#define MARIO_STATE_SIT_RELEASE		601

#define MARIO_STATE_GOIN_PIPE		602

#define MARIO_TURN_TIME 500

#define GROUND_Y 160.0f


#define MARIO_BIG_BBOX_WIDTH  14
#define MARIO_BIG_BBOX_HEIGHT 24
#define MARIO_BIG_SITTING_BBOX_WIDTH  13
#define MARIO_BIG_SITTING_BBOX_HEIGHT 14

#define MARIO_SIT_HEIGHT_ADJUST ((MARIO_BIG_BBOX_HEIGHT-MARIO_BIG_SITTING_BBOX_HEIGHT)/2)

#define MARIO_SMALL_BBOX_WIDTH  12
#define MARIO_SMALL_BBOX_HEIGHT 14

// time = animation time (sum of all frame duration)
#define ATTACK_TIME	210
#define GLIDE_TIME	267
#define FLY_TIME	267
#define TURN_TIME	75
#define PROTECT_TIME	267

#define UNTOUCHABLE_TIME 3000


// for Pipe
#define DISTANCEGODOWNPIPE 40
#define SPEED_IN_PIPE 0.01f

#define PIPE_RANGE 14
#define DISTANCE_GO_DOWN_PIPE 10
#define DISTANCE_GO_UP_PIPE 6

const float MARIO_JUMP_SPEED[4] = { 0.20625f, 0.21375f, 0.22125f, 0.23625f };
const float MARIO_JUMP_SPEED_CHECK_X[3] = { 0.06f, 0.12f, 0.18f};


class CMario : public CCharacter
{
protected:
	vector<pair<float, float>> PipeLocation{
		{2335, 391},
		{2990, 252}
	}; // type % 2 == 0 ? OutDown : OutUp di len la so chan, di xuong la so le
	static unordered_map<MarioLevel, std::unordered_map<MarioAnimationType, int>> animationMap;
	BOOLEAN isSitting;
	MarioLevel level;
	BOOLEAN isOnPlatform;
	int coin;
	CKoopa* holdingShell;
	vector<CPoint*> points;
	int count = 0;
	bool timesup = false;

	// mario vx speed at jump point, for midair physics
	float jumpVx = 0;

	int dirInput = 0; // 1: right, -1: left
	int jumpInput = 0; // 1: jump, 0: no jump
	int runInput = 0; // 1: run, 0: no run

	//Check to go down - up in pipe and distance, press
	bool DownPress, UpPress = false;
	bool GoDownPipe, GoUpPipe = false;
	bool OutDownPipe, OutUpPipe = false;
	bool RenderMarioInPipe = false;
	float DistancePipeGo = 0;
	float DistancePipeOut = 0;
	int tempState;
	int typepipe;
	// timers for animations
	CTimer *attackTimer, *glideTimer, *flyTimer, *untouchableTimer, *turnHoldTimer, *shellProtectTimer;

	int currentAnimation = -1;

	void OnCollisionWithCharacter(LPCOLLISIONEVENT e);
	void OnCollisionWithBaseBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithCoin(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);
	void OnCollisionWithPlant(LPCOLLISIONEVENT e);
	void OnCollisionWithFireball(LPCOLLISIONEVENT e);
	void OnCollisionWithMushroom(LPCOLLISIONEVENT e);
    void OnCollisionWithLeaf(LPCOLLISIONEVENT e);
	void OnCollisionWithPipe(LPCOLLISIONEVENT e);
	void OnCollisionWithSwitch(LPCOLLISIONEVENT e);
	void GetAniId();
	void GetAniIdInPipe();
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
		glideTimer = new CTimer(GLIDE_TIME);
		flyTimer = new CTimer(FLY_TIME);
		attackTimer = new CTimer(ATTACK_TIME);
		untouchableTimer = new CTimer(UNTOUCHABLE_TIME);
		turnHoldTimer = new CTimer(TURN_TIME);
		shellProtectTimer = new CTimer(PROTECT_TIME);
		
		isSitting = false;
		maxVx = 0.0f;
		maxVy = 0.3f;
		ax = 0.0f;
		ay = MARIO_GRAVITY;
		level = MarioLevel::SMALL;
		isOnPlatform = false;
		coin = 0;
		canHold = false;

		holdingShell = NULL;

		SetState(MARIO_STATE_IDLE);
		points.resize(7); // top, left, leftdown, downleft, dowmright, rightdown, rightup
		for (int i = 0; i < 7; i++) {
			points[i] = new CPoint(0, 0);
		}
	}

	~CMario() {
		for (CPoint* point : points) {
			delete point; // Free each dynamically allocated CPoint
		}
		points.clear(); // Clear the vector
	}

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);

	void Acceleration(DWORD dt);

	int IsCollidable()
	{
		return (state != MARIO_STATE_DIE);
	}

	int IsBlocking() { return 0; }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void SetLevel(MarioLevel l);
	void StartUntouchable() { untouchableTimer->Start(); }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void GetTailHitBox(float& l1, float& t1, float& r1, float& b1, float& l2, float& t2, float& r2, float& b2);
	void SetCanHold(bool pick) { this->canHold = pick; }
	bool GetHolding() { return holdingShell != NULL; }
	void Drop() { holdingShell = NULL; }
	void HoldingProcess(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Attacked();
	virtual void Stomped() {};
	virtual void ShellHit(int shellX) {};
	virtual void TailHit(float x) {};
	virtual void BlockHit() {};

	void ResetCurrentAnimation() {
		if(currentAnimation > 0) {
			CAnimations::GetInstance()->Get(currentAnimation)->Reset();
		}
	
	}

	void SkipCurrentAnimation() {
		if(currentAnimation > 0) {
			CAnimations::GetInstance()->Get(currentAnimation)->Skip();
		}
	}

	void TailAttackInit();
	void TailAttack(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	void KickedShell();

	void SpecialPressed();
	void JumpPressed();

	MarioLevel GetLevel() { return level; }
	bool IsBig() { return level >= MarioLevel::BIG; }
	bool IsRaccoon() { return level == MarioLevel::RACCOON; }
	bool IsOnPlatform() { return isOnPlatform; }
	bool IsPMeterFull();

	void SetJumpInput(int jump) { this->jumpInput = jump; }
	int GetJumpInput() { return jumpInput; }

	void SetRunInput(int run) { this->runInput = run; }
	int GetRunInput() { return runInput; }

	void SetDirInput(int dir) { this->dirInput = dir; }
	int GetDirInput() { return dirInput; }

	void SetPointsPosition();

	void StartHoldTurn(int nx) {
		if(this->nx != nx) turnHoldTimer->Start();
	}

	bool GetTimesUp() { return timesup; }
	void SetTimesUp() { timesup = true; }

	void SetSpeed(float vx, float vy) {
		this->vx = vx;
		this->vy = vy;
		for (int i = 0; i < 7; i++) {
			points[i]->SetSpeed(vx, vy);
		}
	}
	void SetPressDown() { DownPress = true; }
	void SetPressUp() {  UpPress = true; }
	void ReleasePress() { DownPress = UpPress = false; }
	bool ReturnRenderMarioInPipe() { return RenderMarioInPipe; }

	void GoingPipe(DWORD dt);

	CPoint* GetPoint(int id) {
		if (id < 0 || id >= points.size()) return NULL;
		return points[id];
	}
	
	//bool DownPress() { return DownPress; }
	//bool UpPress() { return UpPress; }
};

//GROUND PHYSICS
//
//Flat Ground Physics
//
//px / f
//Max velocity(walking) - 1.5
//Max velocity(running) - 2.5
//Max velocity(sprinting) - 3.5
//End - of - level walk speed - 1.25
//Airship cutscene walk speed - 2
//
//px / f ^ 2
//Walk / run / sprint acceleration - 0.0546875
//Stop Deceleration(normal) - 0.0546875
//Stop Deceleration(ice) - 0.01171875
//Skid Deceleration(normal) - 0.125
//Skid Deceleration(ice) - 0.046875
//
//Sloped Ground Physics
//
//px / f
//Uphill maximum walk speed - 0.8125
//Uphill maximum run speed - 1.375
//Sliding max speed - 3.9375
//
//px / f ^ 2
//Added downhill speed(26 deg) - 0.125
//Added downhill speed(45 deg) - 0.1875
//Sliding accel / decel(26 deg) - 0.0625
//Sliding accel / decel(45 deg) - 0.125
//
//MIDAIR PHYSICS
//
//If Small, Big, Fiery or Hammer Mario :
//-Releasing B and /or Left / Right has no effect on your speed.
//- Holding the direction of motion accelerates you at a rate of 0.0546875 px / f ^ 2 (up to 1.5, 2.5, or 3.5 px / f)
//- Holding the direction opposite motion decelerates you at a rate of 0.125 px / f ^ 2
//
//If Raccoon or Tanooki Mario and NOT in flight mode,
//-Releasing Left / Right decelerates you at a rate of about 0.0625 px / f ^ 2 until you reach a speed of 1.4375 px / f
//- Holding the direction of motion accelerates you at a rate of 0.0546875 px / f ^ 2 (up to 1.5 or 2.5 px / f)
//- Holding the direction opposite motion decelerates you at a rate of 0.1875 px / f ^ 2
//
//If Raccoon or Tanooki Mario and in flight mode,
//-Even when holding B and the direction of flight, you will decelerate at a rate of 0.015625 px / f ^ 2 until you reach a speed of 1.4375 px / f
//- Releasing Left / Right decelerates you at a rate of 0.0625 px / f ^ 2 until you reach a speed of 1.4375 px / f
//- Holding the direction of motion accelerates you at a rate of 0.0546875 px / f ^ 2 (up to 1.4375 px / f)
//- Holding the direction opposite motion decelerates you at a rate of 0.1875 px / f ^ 2
//
//JUMP PHYSICS
//
//px / f
//Jump, X Velocity < 1 px / f(-Y)  3.4375
//	Jump, X Velocity > 1 px / f(-Y)  3.5625 
//	Jump, X Velocity > 2 px / f(-Y)  3.6875 
//	Jump, X Velocity > 3 px / f(-Y)  3.9375
//	Enemy Stomp Speed(-Y)          4
//	Weak Enemy Stomp(-Y)           3
//	Maximum Downward Speed(+Y)     4.3125
//	Raccoon Flying(-Y)             1.5
//	Raccoon Slowed Descent(+Y)     1
//
//	px / f ^ 2
//	Jump Gravity
//	(A held, Y Vel < -2 px / f)     0.0625
//	(Y Vel > -2 px / f)             0.3125
//
//	SWIMMING PHYSICS
//
//	px / f
//	Underwater max walk speed       1
//	Maximum underwater Y - speed(+Y) 2
//	Max swimming speed              3
//	Jump out of water init vel(-Y) 3.1875
//
//	px / f ^ 2
//	Y Acceleration(moving up)      0.0625
//	Y Acceleration(moving down)    0.03125
//	Y Acceleration(at surface, +Y) 0.046875
//	X Acceleration(get to speed)   0.0234375
//	X Acceleration(no direction)   0.0078125
//	X Acceleration(turning around) 0.03125