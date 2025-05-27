#include "Sprite.h"

#include "Game.h"

CSprite::CSprite(int id, int left, int top, int right, int bottom, LPTEXTURE tex, bool onScreen, float offsetX, float offsetY)
{
	this->id = id;
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	this->texture = tex;
	this->onScreen = onScreen;
	this->offsetX = offsetX;
	this->offsetY = offsetY;

	float texWidth = (float)tex->getWidth();
	float texHeight = (float)tex->getHeight();

	// Set the sprite�s shader resource view
	sprite.pTexture = tex->getShaderResourceView();

	sprite.TexCoord.x = this->left / texWidth;
	sprite.TexCoord.y = this->top / texHeight;

	int spriteWidth = (this->right - this->left + 1);
	int spriteHeight = (this->bottom - this->top + 1);

	sprite.TexSize.x = spriteWidth / texWidth;
	sprite.TexSize.y = spriteHeight / texHeight;

	sprite.ColorModulate = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	sprite.TextureIndex = 0;

	D3DXMatrixScaling(&this->matScaling, (FLOAT)spriteWidth, (FLOAT)spriteHeight, 1.0f);
}

CSprite::~CSprite()
{
}

void CSprite::Draw(float x, float y)
{
	if(onScreen)
		DrawOnScreen(x, y);
	else
		DrawOnCamera(x, y);
}

void CSprite::DrawOnCamera(float x, float y)
{
	CGame* g = CGame::GetInstance();
	float cx, cy;
	g->GetCurrentScene()->GetCamera()->GetPosition(cx, cy);

	D3DXMATRIX matTranslation;
	x = (FLOAT)floor(x);
	cx = (FLOAT)floor(cx);

	D3DXMatrixTranslation(&matTranslation, x - cx + offsetX, (FLOAT)floor(g->GetBackBufferHeight() - y + cy - offsetY), 0.1f);

	this->sprite.matWorld = (this->matScaling * matTranslation);

	g->GetSpriteHandler()->DrawSpritesImmediate(&sprite, 1, 0, 0);
}

void CSprite::DrawOnScreen(float x, float y)
{
	CGame* g = CGame::GetInstance();
	D3DXMATRIX matTranslation;

	x = (FLOAT)floor(x);
	y = (FLOAT)floor(y);

	D3DXMatrixTranslation(&matTranslation, x + offsetX, g->GetBackBufferHeight() - y - offsetY, 0.1f);

	this->sprite.matWorld = (this->matScaling * matTranslation);

	g->GetSpriteHandler()->DrawSpritesImmediate(&sprite, 1, 0, 0);
}

