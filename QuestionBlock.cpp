#include "QuestionBlock.h"

void CQuestionBlock::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (isUnbox)
		animations->Get(ID_ANI_BLOCK_UNBOX)->Render(x, y);
	else
	animations->Get(ID_ANI_BLOCK_NORMAL)->Render(x, y);
	RenderBoundingBox();
}

void CQuestionBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{	
	if (startY - y > DISTANCE_UP)
	{
		SetState(QUESTION_BLOCK_STATE_MOVEDOWN);
	}
	if (vy > 0 && y >= startY) {
		SetState(QUESTION_BLOCK_STATE_UNBOX);
		y = startY;
		vy = 0;
	}
	y += vy * dt;
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CQuestionBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - BRICK_BBOX_WIDTH / 2;
	t = y - BRICK_BBOX_HEIGHT / 2;
	r = l + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;
}

void CQuestionBlock::SetState(int state)
{
	LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	CGameObject::SetState(state);
	switch (state)
	{
	case QUESTION_BLOCK_STATE_ITEM:
		vy = 0;
		break;
	case QUESTION_BLOCK_STATE_MOVEUP:
		vy = -SPEED_QUESTION_BLOCK;
		isUnbox = true;
		if (type == ITEM_COIN)
		{
			CCoin* coin = new CCoin(x, y);
			coin->SetState(COIN_STATE_MOVEUP);
			scene->AddObject(coin);
		}
		break;
	case QUESTION_BLOCK_STATE_MOVEDOWN:
		vy = SPEED_QUESTION_BLOCK;
		break;
	case QUESTION_BLOCK_STATE_UNBOX:
		if (type == ITEM_RED_MUSHROOM)
		{
			CMushroom* mushroom = new CMushroom(x, y - DISTANCE_SPAWN_MUSHROOM, ITEM_RED_MUSHROOM);
			mushroom->SetState(MUSHROOM_STATE_UP);
			scene->AddObject(mushroom);
		}
		
		break;
	}
}

void CQuestionBlock::OnCollisionWith(LPCOLLISIONEVENT e, DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{	

	//CMushroom* mushroom = dynamic_cast<CMushroom*>(e->obj);
	//if (e->ny > 0) 
	//{
	//	if (mushroom->GetState() == MUSHROOM_STATE_WALKING /*&& GetState() == QUESTION_BLOCK_STATE_MOVEUP*/) 
	//	{
	//		mushroom->SetState(MUSHROOM_STATE_BOUNCING);
	//	}
	//}
}
