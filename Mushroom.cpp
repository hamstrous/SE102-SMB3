#include "Mushroom.h"
#include "QuestionBlock.h"
void CMushroom::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
		left = x - MUSHROOM_BBOX_WIDTH / 2;
		top = y - MUSHROOM_BBOX_HEIGHT / 2;
		right = left + MUSHROOM_BBOX_WIDTH;
		bottom = top + MUSHROOM_BBOX_HEIGHT;
}

void CMushroom::GetHaftBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - MUSHROOM_BBOX_WIDTH / 2;
	top = y;
	right = left + MUSHROOM_BBOX_WIDTH;
	bottom = top + MUSHROOM_BBOX_HEIGHT_PLUS / 2 ;
}

void CMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{	
	if (state == MUSHROOM_STATE_UP && OnFloor(dt, coObjects) == 1 && dir == true) {
		SetState(MUSHROOM_STATE_WALKING_RIGHT);
	}
	else if (state == MUSHROOM_STATE_UP && OnFloor(dt, coObjects) == 1 && dir == false) {
			SetState(MUSHROOM_STATE_WALKING_LEFT);
	}
	/*if (state == MUSHROOM_STATE_BOUNCING && OnFloor(dt, coObjects) == 0 && dir == true) {
		SetState(MUSHROOM_STATE_WALKING_RIGHT);
	}
	else if (state == MUSHROOM_STATE_BOUNCING && OnFloor(dt, coObjects) == 0 && dir == false) {
		SetState(MUSHROOM_STATE_WALKING_LEFT);
	}*/
	vy += ay * dt;
	vx += ax * dt;
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMushroom::Render()
{
	int aniId = ID_ANI_MUSHROOM_WALKING;
	if (type == TYPE_LEVELUP)
		aniId = ID_ANI_MUSHROOM_1UP_WALKING;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CMushroom::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CMushroom::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CMushroom*>(e->obj)) return;
	if (e->ny != 0)
	{
		if (e->ny < 0) 
		{
			if (dynamic_cast<CQuestionBlock*>(e->obj))
			{
				CQuestionBlock* qb = dynamic_cast<CQuestionBlock*>(e->obj);
				if (qb->GetState() == QUESTION_BLOCK_STATE_MOVEUP)
				{
					SetState(MUSHROOM_STATE_BOUNCING);
					return;
				}
			}
			vy = 0;
		}
		else 
		{
			vy = 0;
		}
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

int CMushroom::OnFloor(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float ml, mt, mr, mb;
	GetHaftBoundingBox(ml, mt, mr, mb);
	return CCollision::GetInstance()->CheckStillTouchSolid(ml, mt, mr, mb, vx, vy, dt, coObjects);
}

void CMushroom::InitHorizontalSpeed(float speed, float awayMario)
{

}

void CMushroom::SetState(int state)
{	
	CGameObject::SetState(state);
	switch (state)
	{
	case MUSHROOM_STATE_WALKING_RIGHT:
		vx = MUSHROOM_SPEED;
		ay = MUSHROOM_GRAVITY;
		break;
	case MUSHROOM_STATE_WALKING_LEFT:
		vx = -MUSHROOM_SPEED;
		ay = MUSHROOM_GRAVITY;
		break;
	case MUSHROOM_STATE_UP:
		vx = 0;
		ay = -MUSHROOM_GRAVITY/10;
		break;
	case MUSHROOM_STATE_BOUNCING:
		vy = -MUSHROOM_BOUNCING;
		ay = MUSHROOM_GRAVITY;   
		vx = MUSHROOM_SPEED_BOUNCING;
		break;
	case MUSHROOM_STATE_DELETE:
	{
		CScore* score = new CScore(x, y - MUSHROOM_BBOX_HEIGHT, SCORE_1000);
		score->SetState(SCORE_STATE_UP);
		((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->AddObject(score);
		Delete();
		break;
	}
	default:
		break;
	}
}
