#include "QuestionBlock.h"

void CQuestionBlock::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (isUnbox)
		animations->Get(ID_ANI_BLOCK_UNBOX)->Render(x, y);
	else
	animations->Get(ID_ANI_BLOCK_NORMAL)->Render(x, y);
	//RenderBoundingBox();
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
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	float marioX, marioY;
	mario->GetPosition(marioX, marioY);
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
		if (type == ITEM_RED_MUSHROOM && mario->GetLevel() == MarioLevel::BIG) {
			CLeaf* leaf = new CLeaf(x, y - DISTANCE_SPAWN - 15);
			leaf->SetState(LEAF_STATE_UP);
			scene->AddObject2(leaf, 1);
		}
		break;
	case QUESTION_BLOCK_STATE_MOVEDOWN:
		vy = SPEED_QUESTION_BLOCK;
		break;
	case QUESTION_BLOCK_STATE_UNBOX:
		if (type == ITEM_RED_MUSHROOM && mario->GetLevel() == MARIO_LEVEL_SMALL)
		{	
			bool dir = (x > marioX) ? true : false;
			int type = (mario->GetLevel() == MarioLevel::BIG) ? ITEM_GREEN_MUSHROOM : ITEM_RED_MUSHROOM;
			CMushroom* mushroom = new CMushroom(x, y - DISTANCE_SPAWN, type, dir);
			mushroom->SetState(MUSHROOM_STATE_UP);
			scene->AddObject2(mushroom, 14);
		}
		if (type == ITEM_GREEN_MUSHROOM)
		{
			bool dir = (x > marioX) ? true : false;
			CMushroom* mushroom = new CMushroom(x, y - DISTANCE_SPAWN, ITEM_GREEN_MUSHROOM, dir);
			mushroom->SetState(MUSHROOM_STATE_UP);
			scene->AddObject2(mushroom, 14);
		}
		break;
	}
}

void CQuestionBlock::OnCollisionWith(LPCOLLISIONEVENT e)
{	
	
}

void CQuestionBlock::SideHit()
{
	if (state == QUESTION_BLOCK_STATE_ITEM)
	{
		SetState(QUESTION_BLOCK_STATE_MOVEUP);
	}
}

void CQuestionBlock::BottomHit()
{
	if (state == QUESTION_BLOCK_STATE_ITEM)
	{
		SetState(QUESTION_BLOCK_STATE_MOVEUP);
	}
}


