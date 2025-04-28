#include "KoopaRed.h"
#include "ColorBlock.h"
#include "BackgroundColor.h"
#include "debug.h"

void CKoopaRed::GetFloorBoundingBox(float& left, float& top, float& right, float& bottom)
{
	top = y + KOOPA_BBOX_HEIGHT / 2;
	bottom = top + KOOPA_FLOOR_CHECK_BBOX_HEIGHT;
	if (vx > 0) {
		left = x + KOOPA_BBOX_WIDTH / 2;
		right = left + KOOPA_FLOOR_CHECK_BBOX_WIDTH;
	}
	else {
		right = x - KOOPA_BBOX_WIDTH / 2;
		left = right - KOOPA_FLOOR_CHECK_BBOX_WIDTH;
	}
}


int CKoopaRed::OnFloor(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float ml, mt, mr, mb;
	GetFloorBoundingBox(ml, mt, mr, mb);
	return CCollision::GetInstance()->CheckStillTouchSolid(ml, mt, mr, mb, vx, vy, dt, coObjects);
}


void CKoopaRed::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (vy < 0) {
		float tx, ty;
		e->obj->GetPosition(tx, ty);
		if(dynamic_cast<CBackgroundColor*>(e->obj)) DebugOut(L"%f %f",tx, ty);
	}

	if (dynamic_cast<CCharacter*>(e->obj)) {
		OnCollisionWithCharacter(e);
		return;
	}
	if (dynamic_cast<CQuestionBlock*>(e->obj)) {
		OnCollisionWithQuestionBlock(e);
		
	}
	if (!e->obj->IsBlocking()) return;

	if (e->ny != 0 && !hasWing)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CKoopaRed::OnCollisionWithQuestionBlock(LPCOLLISIONEVENT e)
{
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CMario* player = dynamic_cast<CMario*>(scene->GetPlayer());
	CQuestionBlock* questionblock = (CQuestionBlock*)e->obj;
	if (questionblock->GetState() == QUESTION_BLOCK_STATE_MOVEUP) {
		
			if (state == KOOPA_STATE_WALKING) {
				float xx, yy;
				player->GetPosition(xx, yy);
				if (xx < x) {
					vx = KOOPA_FLYING_SPEED_X;
				}
				else {
					vx = -KOOPA_FLYING_SPEED_X;
				}
				//ShellHit(nx);
				vy = -100.0f;
				//DebugOut(L"vy cua hit die vy: %f\n", vy);
				////hit = true;
				//hasWing = false;
				//delete_time = GetTickCount64();
			}
		
	}
	if (questionblock->GetState() != QUESTION_BLOCK_STATE_UNBOX) {
		if (state == KOOPA_STATE_SHELL_MOVING) {
			questionblock->SetState(QUESTION_BLOCK_STATE_MOVEUP);
		}
	}
}

void CKoopaRed::Walking(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == KOOPA_STATE_WALKING && OnFloor(dt, coObjects) == 1) {
		vx = -vx;
	}

	if (vx > 0) nx = 1;
	else if (vx < 0) nx = -1;

	if (isIdle && GetTickCount64() - shell_start > KOOPA_SHELL_COOLDOWN) {
		SetState(KOOPA_STATE_WALKING);
	}
}
void CKoopaRed::Flying()
{
	if (GetTickCount64() - fly_start > KOOPA_FLY_CHANGE_DIRECTION_TIME) {
		vy = -vy;
		fly_start = GetTickCount64();
	}
}

void CKoopaRed::SetState(int state)
{	
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CMario* player = dynamic_cast<CMario*>(scene->GetPlayer());
	if (this->state == KOOPA_STATE_SHELL_HELD) {
		ay = KOOPA_GRAVITY;
	}
	else if (this->state == KOOPA_STATE_FLYING) {
		ay = KOOPA_GRAVITY;
	}

	switch (state)
	{
	case KOOPA_STATE_SHELL_IDLE:
		if (this->state == KOOPA_STATE_WALKING) y = (y + KOOPA_BBOX_HEIGHT / 2) - KOOPA_BBOX_HEIGHT_SHELL / 2; // when start walking, move up to normal y so dont drop through floor
		// when start shell idle, move down to shell y so dont float above ground
		vx = 0;
		shell_start = GetTickCount64();
		isIdle = true;
		break;
	case KOOPA_STATE_WALKING:
		isIdle = false;
		Release(false); //call to make sure shell is released (mario not holding)
		if (this->state == KOOPA_STATE_SHELL_IDLE) {
			y = (y + KOOPA_BBOX_HEIGHT_SHELL / 2) - KOOPA_BBOX_HEIGHT / 2; // when start walking, move up to normal y so dont drop through floor
			InitHorizontalSpeedBasedOnMario(KOOPA_WALKING_SPEED, -1);
			break;
		}
		vx = KOOPA_WALKING_SPEED * nx;
		// when start walking, walk toward mario
		break;
	case KOOPA_STATE_SHELL_MOVING:
		isIdle = false;
		InitHorizontalSpeedBasedOnMario(KOOPA_SHELL_SPEED); // when kicked, move away from mario
		break;
	case KOOPA_STATE_SHELL_HELD:
		vx = 0;
		vy = 0;
		ay = 0;
		break;
	case KOOPA_STATE_FLYING:
		ay = 0;
		vy = KOOPA_FLYING_SPEED;
		fly_start = GetTickCount64();
		break;
	case KOOPA_STATE_DIE:
		isDeleted = true;
		Release(true);
		break;
	case KOOPA_STATE_DIE_UP: //when hold shell, vy = mario speed
		Release(true);
		float player_x, player_y;
		player->GetSpeed(player_x, player_y);
		vx = player_x;
		
		vy = -KOOPA_STATE_FLYING_UP;
		DebugOut(L"vy cua die up vy: %f\n", vy);
		hit = true;
		hasWing = false;
		delete_time = GetTickCount64();
		break;
	case KOOPA_STATE_HIT_DIE: //when question block hit
		float xx, yy;
		player->GetPosition(xx, yy);
		if (xx < x) {
			vx = KOOPA_FLYING_SPEED_X;
			vy = -2.0f;
		}
		else {
			vx = -KOOPA_FLYING_SPEED_X;
			vy = -2.0f;
		}
		
		vy = -2.0f;
		DebugOut(L"vy cua hit die vy: %f\n", vy);
		hit = true;
		hasWing = false;
		delete_time = GetTickCount64();
		break;
	}
	CGameObject::SetState(state);

}

void CKoopaRed::Render()
{
	float shellTime = GetTickCount64() - shell_start;
	int aniId = 0;
	switch (state) {
	case KOOPA_STATE_FLYING:
		if (nx == 1) aniId = ID_ANI_KOOPA_WALKING_RIGHT;
		else aniId = ID_ANI_KOOPA_WALKING_LEFT;
		break;

	case KOOPA_STATE_WALKING:
		if (vx > 0) aniId = ID_ANI_KOOPA_WALKING_RIGHT;
		else if (vx < 0) aniId = ID_ANI_KOOPA_WALKING_LEFT;
		else aniId = ID_ANI_KOOPA_WALKING_RIGHT;
		break;
	case KOOPA_STATE_SHELL_HELD:
	case KOOPA_STATE_SHELL_IDLE:
		if (isIdle && shellTime > KOOPA_SHELL_COOLDOWN_VIBRATION && shellTime <= KOOPA_SHELL_COOLDOWN_VIBRATION_LEG) {
			aniId = ID_ANI_KOOPA_SHELL_VIBRATING;
		}
		else if (isIdle && GetTickCount64() - shell_start > KOOPA_SHELL_COOLDOWN_VIBRATION_LEG) {
			aniId = ID_ANI_KOOPA_SHELL_VIBRATING_LEG;
		}
		else {
			aniId = ID_ANI_KOOPA_SHELL_IDLE;
		}
		break;
	case KOOPA_STATE_SHELL_MOVING:
		aniId = ID_ANI_KOOPA_SHELL_MOVING;
		break;
	case KOOPA_STATE_DIE_UP_ANI:
		aniId = ID_ANI_KOOPA_DIE_UP;
		break;
	case KOOPA_STATE_DIE_UP:
		aniId = ID_ANI_KOOPA_DIE_UP;
		break;
	case KOOPA_STATE_HIT_DIE:
		aniId = ID_ANI_KOOPA_DIE_UP;
		break;
	default:
		aniId = ID_ANI_KOOPA_WALKING_LEFT;
		break;
	}
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	if (hasWing && nx == 1) CAnimations::GetInstance()->Get(ID_ANI_KOOPA_WING_RIGHT)->Render(x - 3, y - 3);
	else if (hasWing && nx == -1) CAnimations::GetInstance()->Get(ID_ANI_KOOPA_WING_LEFT)->Render(x + 3, y - 3);
	RenderBoundingBox();
}

CKoopaRed::CKoopaRed(float x, float y, bool hasWing) : CKoopa(x, y)
{
	this->x = x;
	this->y = y;
	ax = 0;
	ay = KOOPA_GRAVITY;

	this->hasWing = hasWing;

	if (!hasWing) {
		SetState(KOOPA_STATE_WALKING);
		InitHorizontalSpeedBasedOnMario(KOOPA_WALKING_SPEED);
	}
	else {
		nx = -1;
		SetState(KOOPA_STATE_FLYING);
	}

	isIdle = false;
}

void CKoopaRed::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == KOOPA_STATE_WALKING || state == KOOPA_STATE_FLYING)
	{
		left = x - KOOPA_BBOX_WIDTH / 2;
		top = y - KOOPA_BBOX_HEIGHT / 2;
		right = left + KOOPA_BBOX_WIDTH;
		bottom = top + KOOPA_BBOX_HEIGHT;
	}
	else {
		left = x - KOOPA_BBOX_WIDTH / 2;
		top = y - KOOPA_BBOX_HEIGHT_SHELL / 2;
		right = left + KOOPA_BBOX_WIDTH;
		bottom = top + KOOPA_BBOX_HEIGHT_SHELL;
	}
	if (hit)
		left = top = right = bottom = 0;
}

void CKoopaRed::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
	DebugOutTitle(L"%f\n", vy);
};

void CKoopaRed::OnCollisionWithCharacter(LPCOLLISIONEVENT e)
{
	CCharacter* character = dynamic_cast<CCharacter*>(e->obj);
	if (state == KOOPA_STATE_SHELL_MOVING) {
		// if hit another moving shell, then both get shell hit
		// call this Koppa shell hit first, else no effect
		if (dynamic_cast<CKoopa*>(character) && dynamic_cast<CKoopa*>(character)->GetState() == KOOPA_STATE_SHELL_MOVING) {
			ShellHit(-e->nx);
		}
		character->ShellHit(e->nx);
	}
	else if (state == KOOPA_STATE_SHELL_HELD) {
		HeldDie();
		character->ShellHit(e->nx);
	}
	else if (state == KOOPA_STATE_WALKING) vx = -vx;
}

void CKoopaRed::InitHorizontalSpeedBasedOnMario(float speed, float towardMario)
{
	// if towardMario == 0, then koopa will walk in the same direction as it is facing
	if (towardMario == 0) {
		vx = speed * nx;
		return;
	}
	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	float mario_x = 0, mario_y = 0;
	CMario* mario = (CMario*)scene->GetPlayer();
	if (mario != NULL) mario->GetPosition(mario_x, mario_y);
	if (mario_x <= x) {
		vx = speed * towardMario;
	}
	else {
		vx = -speed * towardMario;
	}
}

void CKoopaRed::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;
	if (hasWing)
		Flying();
	else
		Walking(dt, coObjects);
	if (vx > 0) nx = 1;
	else if (vx < 0) nx = -1;
	if (state == KOOPA_STATE_SHELL_HELD) {
		ShellHeldTouch(dt, coObjects);
	}
	if (state == KOOPA_STATE_DIE_UP_ANI || state == KOOPA_STATE_DIE_UP) {
		if (GetTickCount64() - delete_time > KOOPA_TIME_DELETE) {
			isDeleted = true;
		}
	}
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CKoopaRed::Kicked()
{
	// Only when in shell state can Koopa be kicked by Mario
	if (state == KOOPA_STATE_SHELL_IDLE || state == KOOPA_STATE_SHELL_HELD) {
		SetState(KOOPA_STATE_SHELL_MOVING);
	}
}

void CKoopaRed::Stomped()
{
	if (state == KOOPA_STATE_FLYING) {
		hasWing = false;
		SetState(KOOPA_STATE_WALKING);
	}
	else if (state != KOOPA_STATE_SHELL_IDLE)
	{
		SetState(KOOPA_STATE_SHELL_IDLE);
	}
	else {
		Kicked();
	}
}

void CKoopaRed::Held()
{
	if (state == KOOPA_STATE_SHELL_IDLE) SetState(KOOPA_STATE_SHELL_HELD);
}

void CKoopaRed::Release(bool dead = false)
{
	if (state == KOOPA_STATE_SHELL_HELD) {
		CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		CMario* player = dynamic_cast<CMario*>(scene->GetPlayer());
		if (!dead) vy = -0.3f;
		player->Drop();
	}
}

void CKoopaRed::ShellHit(int shellX)
{
	SetState(KOOPA_STATE_DIE_UP);
	if (shellX == -1) vx = KOOPA_FLYING_SPEED_X;
	else if (shellX == 1) vx = -KOOPA_FLYING_SPEED_X;
	else if (shellX < x) vx = KOOPA_FLYING_SPEED_X;
	else vx = -KOOPA_FLYING_SPEED_X;

	vy = -KOOPA_STATE_FLYING_UP;
	hit = true;
	hasWing = false;
	delete_time = GetTickCount64();
}

void CKoopaRed::Touched()
{
	CPlayScene* scene = (CPlayScene*)(CGame::GetInstance()->GetCurrentScene());
	CMario* mario = dynamic_cast<CMario*>(scene->GetPlayer());
	if (state != KOOPA_STATE_SHELL_IDLE && state != KOOPA_STATE_SHELL_HELD) {
		mario->Attacked();
	}
	else if (state == KOOPA_STATE_SHELL_IDLE) {
		Kicked();
		mario->KickedShell();
	}
}

void CKoopaRed::HeldDie()
{
}

void CKoopaRed::ThrownInBlock(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == KOOPA_STATE_SHELL_MOVING) {
		float ml, mt, mr, mb;
		GetBoundingBox(ml, mt, mr, mb);
		if (CCollision::GetInstance()->CheckTouchingSolid(ml, mt, mr, mb, vx, vy, dt, coObjects)) {
			SetState(KOOPA_STATE_DIE_UP);
		}
	}
}

void CKoopaRed::ShellHeldTouch(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == KOOPA_STATE_SHELL_HELD) {
		float ml, mt, mr, mb;
		GetBoundingBox(ml, mt, mr, mb);
		if (CCollision::GetInstance()->CheckTouchCharacterForShellHeldHit(ml, mt, mr, mb, vx, vy, dt, coObjects, true)) {
			SetState(KOOPA_STATE_DIE_UP);
		}
	}
}
