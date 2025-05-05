#include "QuestionBlock.h"
#include "Mushroom.h"
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
	if (state != QUESTION_BLOCK_STATE_MOVEUP) up = false;

	if (state == QUESTION_BLOCK_STATE_MOVEUP && GetTickCount64() - start_up >= TIME_UP) {
		//DebugOut(L"[INFO] Time up: %d\n", start_up);
		SetState(QUESTION_BLOCK_STATE_MOVEDOWN);
		start_up = -1;
	}
	if (startY - y > DISTANCE_UP )
	{
		
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
		start_up = GetTickCount64();
		vy = -SPEED_QUESTION_BLOCK;
		isUnbox = true;
		up = true;
		if (type == ITEM_COIN)
		{
			CCoin* coin = new CCoin(x, y);
			coin->SetState(COIN_STATE_MOVEUP);
			scene->AddObject(coin);
		}
		if (type == ITEM_RED_MUSHROOM && mario->GetLevel() >= MarioLevel::BIG) {
			CLeaf* leaf = new CLeaf(x, y - DISTANCE_SPAWN - 15);
			leaf->SetState(LEAF_STATE_UP);
			scene->AddObject(leaf);
		}
		break;
	case QUESTION_BLOCK_STATE_MOVEDOWN:
		vy = SPEED_QUESTION_BLOCK;
		break;
	case QUESTION_BLOCK_STATE_UNBOX:
		if (type == ITEM_RED_MUSHROOM && mario->GetLevel() == MarioLevel::SMALL)
		{	
			bool dir = (x > marioX) ? true : false;
			int type = (mario->GetLevel() >= MarioLevel::BIG) ? ITEM_GREEN_MUSHROOM : ITEM_RED_MUSHROOM;
			CMushroom* mushroom = new CMushroom(x, y - DISTANCE_SPAWN, type, dir);
			mushroom->SetState(MUSHROOM_STATE_UP);
			scene->AddObject(mushroom);
		}
		if (type == ITEM_GREEN_MUSHROOM)
		{
			bool dir = (x > marioX) ? true : false;
			CMushroom* mushroom = new CMushroom(x, y - DISTANCE_SPAWN, ITEM_GREEN_MUSHROOM, dir);
			mushroom->SetState(MUSHROOM_STATE_UP);
			scene->AddObject(mushroom);
		}
		break;
	}
}

void CQuestionBlock::OnCollisionWith(LPCOLLISIONEVENT e)
{	
	DebugOut(L"[INFO] OnCollisionWith called\n");
	if (vy != 0)
	{
		DebugOut(L"[INFO] OnCollisionWith: Block is moving\n");
	}
	else
	{
		DebugOut(L"[INFO] OnCollisionWith: Block is not moving\n");
	}
	if (dynamic_cast<CMushroom*>(e->obj))
	{
		DebugOut(L"[INFO] Collided with Mushroom\n");

		CMushroom* mushroom = dynamic_cast<CMushroom*>(e->obj);

		if (mushroom == nullptr)
		{
			DebugOut(L"[ERROR] e->obj is not a valid Mushroom\n");
			return;
		}

		DebugOut(L"[INFO] Mushroom is valid\n");

		if (state == QUESTION_BLOCK_STATE_MOVEUP)
		{
			DebugOut(L"[INFO] QuestionBlock State: %d\n", state);
			mushroom->SetState(MUSHROOM_STATE_BOUNCING_REVERSE);
			return;
		}
	}
	else
	{
		DebugOut(L"[INFO] Collided with another object\n");
	}
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


