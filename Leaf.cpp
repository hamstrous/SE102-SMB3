#include "Leaf.h"

void CLeaf::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - LEAF_BBOX_WIDTH / 2;
	top = y - LEAF_BBOX_HEIGHT / 2;
	right = left + LEAF_BBOX_WIDTH;
	bottom = top + LEAF_BBOX_HEIGHT;

}

void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == LEAF_STATE_UP && startY - y >= LEAF_UP_DISTANCE)
	{
		SetState(LEAF_STATE_STOP);
	}
	if (state == LEAF_STATE_STOP)
	{
		SetState(LEAF_STATE_FALL);
		
	}
	if (state == LEAF_STATE_FALL && GetTickCount64() - fallReverse_start >= LEAF_FALL_REVERSE_TIME)
	{
		vx = -vx;
		fallReverse_start = GetTickCount64();
	}
	vy += ay * dt;
	vx += ax * dt;
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CLeaf::Render()
{	
	if (GetIsPause()) return;
	int aniId = ID_ANI_LEAF_RIGHT;
	if (vx < 0) aniId = ID_ANI_LEAF_LEFT;
	if (!GetIsStop()) CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	else CAnimations::GetInstance()->Get(aniId)->Render(x, y, 1);
	//RenderBoundingBox();
}

void CLeaf::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
	//DebugOut(L"Leaf: %f %f \n", x, y);
}

void CLeaf::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CMario*>(e->obj))
	{
		DebugOut(L"Leaf collided with Mario\n");
		CMario* mario = dynamic_cast<CMario*>(e->obj);
		mario->SetLevel(MarioLevel::RACCOON);
		this->Delete(); 
	}
	else
	{	
		DebugOut(L"Leaf collided with another object\n");
		return;
	}
}

void CLeaf::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case LEAF_STATE_UP:
		vy = -LEAF_SPEED_UP;
		vx = 0;
		ax = 0;
		ay = 0;
		break;
	case LEAF_STATE_FALL:
		vx = LEAF_SPEED;
		ax = 0;
		ay = LEAF_GRAVITY;
		fallReverse_start = GetTickCount64();
		break;
	case LEAF_STATE_STOP:
		vx = 0;
		vy = 0;
		break;
	case LEAF_STATE_DELETE:
	{
		CScore* score = new CScore(x, y, ID_ANI_SCORE_1000);
		score->SetState(SCORE_STATE_UP);
		LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
		scene->AddObject(score);
		Delete();
		break;
	}
	default:
		break;
	}
}
