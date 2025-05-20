#include "Sprites.h"
#include "Game.h"
#include "debug.h"
#include "Textures.h"

CSprites* CSprites::__instance = NULL;

CSprites* CSprites::GetInstance()
{
	if (__instance == NULL) __instance = new CSprites();
	return __instance;
}

void CSprites::Add(int id, int left, int top, int right, int bottom, LPTEXTURE tex, bool onScreen, float offsetX, float offsetY)
{
	LPSPRITE s = new CSprite(id, left, top, right, bottom, tex, onScreen, offsetX, offsetY);
	sprites[id] = s;
}

LPSPRITE CSprites::Get(int id)
{
	return sprites[id];
}

bool CSprites::IsPixelTransparent(DWORD color)
{
	// Check if the alpha channel is 0 (fully transparent)
	return ((color >> 24) & 0xFF) == 0;
}

vector<DWORD> CSprites::GetTexturePixels(LPTEXTURE tex, int left, int top, int right, int bottom)
{
	vector<DWORD> pixels;
	
	// Get texture description
	D3D10_TEXTURE2D_DESC desc;
	desc.Width = tex->getWidth();
	desc.Height = tex->getHeight();

	// Create a staging texture to read from
	ID3D10Texture2D* stagingTexture;
	D3D10_TEXTURE2D_DESC stagingDesc;
	stagingDesc.Width = desc.Width;
	stagingDesc.Height = desc.Height;
	stagingDesc.MipLevels = 1;
	stagingDesc.ArraySize = 1;
	stagingDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	stagingDesc.SampleDesc.Count = 1;
	stagingDesc.SampleDesc.Quality = 0;
	stagingDesc.Usage = D3D10_USAGE_STAGING;
	stagingDesc.BindFlags = 0;
	stagingDesc.CPUAccessFlags = D3D10_CPU_ACCESS_READ;
	stagingDesc.MiscFlags = 0;
	
	HRESULT hr = CGame::GetInstance()->GetDirect3DDevice()->CreateTexture2D(&stagingDesc, NULL, &stagingTexture);
	if (FAILED(hr)) return pixels;

	// Get the source texture from the shader resource view
	ID3D10Resource* sourceResource;
	tex->getShaderResourceView()->GetResource(&sourceResource);
	ID3D10Texture2D* sourceTexture = (ID3D10Texture2D*)sourceResource;

	// Copy the texture data to staging texture
	CGame::GetInstance()->GetDirect3DDevice()->CopyResource(stagingTexture, sourceTexture);
	sourceResource->Release();

	// Map the staging texture to read the pixels
	D3D10_MAPPED_TEXTURE2D mappedTexture;
	hr = stagingTexture->Map(0, D3D10_MAP_READ, 0, &mappedTexture);
	if (FAILED(hr)) {
		stagingTexture->Release();
		return pixels;
	}

	// Read the pixels
	for (int y = top; y < bottom; y++) {
		for (int x = left; x < right; x++) {
			if (x >= 0 && x < (int)desc.Width && y >= 0 && y < (int)desc.Height) {
				DWORD* pixel = (DWORD*)((BYTE*)mappedTexture.pData + y * mappedTexture.RowPitch + x * sizeof(DWORD));
				pixels.push_back(*pixel);
			}
		}
	}

	// Unmap and release the staging texture
	stagingTexture->Unmap(0);
	stagingTexture->Release();

	return pixels;
}

LPSPRITE CSprites::CreateOverlapSprite(LPSPRITE background, LPSPRITE mario, float bgX, float bgY, float marioX, float marioY)
{
	// Get sprite dimensions
	int bgLeft, bgTop, bgRight, bgBottom;
	int marioLeft, marioTop, marioRight, marioBottom;

	background->GetBoundingBox(bgLeft, bgTop, bgRight, bgBottom);
	mario->GetBoundingBox(marioLeft, marioTop, marioRight, marioBottom);

	// Calculate world positions
	float bgWorldLeft = bgX - (bgRight - bgLeft) / 2.0f;
	float bgWorldTop = bgY - (bgBottom - bgTop) / 2.0f;
	float bgWorldRight = bgX + (bgRight - bgLeft) / 2.0f;
	float bgWorldBottom = bgY + (bgBottom - bgTop) / 2.0f;

	float marioWorldLeft = marioX - (marioRight - marioLeft) / 2.0f;
	float marioWorldTop = marioY - (marioBottom - marioTop) / 2.0f;
	float marioWorldRight = marioX + (marioRight - marioLeft) / 2.0f;
	float marioWorldBottom = marioY + (marioBottom - marioTop) / 2.0f;

	// Calculate overlap region
	float overlapLeft = max(bgWorldLeft, marioWorldLeft);
	float overlapTop = max(bgWorldTop, marioWorldTop);
	float overlapRight = min(bgWorldRight, marioWorldRight);
	float overlapBottom = min(bgWorldBottom, marioWorldBottom);

	// If no overlap, return NULL
	if (overlapLeft >= overlapRight || overlapTop >= overlapBottom)
		return NULL;

	// Convert world coordinates to sprite coordinates
	float bgWidth = bgRight - bgLeft;
	float bgHeight = bgBottom - bgTop;
	float marioWidth = marioRight - marioLeft;
	float marioHeight = marioBottom - marioTop;

	// Calculate sprite coordinates for the overlap region
	int bgOverlapLeft = bgLeft + (int)((overlapLeft - bgWorldLeft) * bgWidth / (bgWorldRight - bgWorldLeft));
	int bgOverlapTop = bgTop + (int)((overlapTop - bgWorldTop) * bgHeight / (bgWorldBottom - bgWorldTop));
	int bgOverlapRight = bgLeft + (int)((overlapRight - bgWorldLeft) * bgWidth / (bgWorldRight - bgWorldLeft));
	int bgOverlapBottom = bgTop + (int)((overlapBottom - bgWorldTop) * bgHeight / (bgWorldBottom - bgWorldTop));

	int marioOverlapLeft = marioLeft + (int)((overlapLeft - marioWorldLeft) * marioWidth / (marioWorldRight - marioWorldLeft));
	int marioOverlapTop = marioTop + (int)((overlapTop - marioWorldTop) * marioHeight / (marioWorldBottom - marioWorldTop));
	int marioOverlapRight = marioLeft + (int)((overlapRight - marioWorldLeft) * marioWidth / (marioWorldRight - marioWorldLeft));
	int marioOverlapBottom = marioTop + (int)((overlapBottom - marioWorldTop) * marioHeight / (marioWorldBottom - marioWorldTop));

	// Get pixel data for both sprites in the overlap region
	vector<DWORD> bgPixels = GetTexturePixels(background->GetTexture(), bgOverlapLeft, bgOverlapTop, bgOverlapRight, bgOverlapBottom);
	vector<DWORD> marioPixels = GetTexturePixels(mario->GetTexture(), marioOverlapLeft, marioOverlapTop, marioOverlapRight, marioOverlapBottom);

	// Create a new texture for the overlap region
	ID3D10Texture2D* overlapTexture;
	D3D10_TEXTURE2D_DESC desc;
	desc.Width = bgOverlapRight - bgOverlapLeft;
	desc.Height = bgOverlapBottom - bgOverlapTop;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D10_USAGE_DEFAULT;
	desc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	HRESULT hr = CGame::GetInstance()->GetDirect3DDevice()->CreateTexture2D(&desc, NULL, &overlapTexture);
	if (FAILED(hr)) return NULL;

	// Create a staging texture to write to
	ID3D10Texture2D* stagingTexture;
	D3D10_TEXTURE2D_DESC stagingDesc = desc;
	stagingDesc.Usage = D3D10_USAGE_STAGING;
	stagingDesc.BindFlags = 0;
	stagingDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;

	hr = CGame::GetInstance()->GetDirect3DDevice()->CreateTexture2D(&stagingDesc, NULL, &stagingTexture);
	if (FAILED(hr)) {
		overlapTexture->Release();
		return NULL;
	}

	// Map the staging texture to write the pixels
	D3D10_MAPPED_TEXTURE2D mappedTexture;
	hr = stagingTexture->Map(0, D3D10_MAP_WRITE, 0, &mappedTexture);
	if (FAILED(hr)) {
		stagingTexture->Release();
		overlapTexture->Release();
		return NULL;
	}

	// Write the pixels, only copying background pixels where Mario has non-transparent pixels
	for (int y = 0; y < desc.Height; y++) {
		for (int x = 0; x < desc.Width; x++) {
			int index = y * desc.Width + x;
			if (index < marioPixels.size() && !IsPixelTransparent(marioPixels[index])) {
				DWORD* pixel = (DWORD*)((BYTE*)mappedTexture.pData + y * mappedTexture.RowPitch + x * sizeof(DWORD));
				*pixel = bgPixels[index];
			}
		}
	}

	// Unmap and copy to the final texture
	stagingTexture->Unmap(0);
	CGame::GetInstance()->GetDirect3DDevice()->CopyResource(overlapTexture, stagingTexture);
	stagingTexture->Release();

	// Create shader resource view for the new texture
	ID3D10ShaderResourceView* rsview;
	hr = CGame::GetInstance()->GetDirect3DDevice()->CreateShaderResourceView(overlapTexture, NULL, &rsview);
	if (FAILED(hr)) {
		overlapTexture->Release();
		return NULL;
	}

	// Create a new texture object
	LPTEXTURE tex = new CTexture(overlapTexture, rsview);

	// Create and return the overlap sprite
	LPSPRITE overlapSprite = new CSprite(
		-1,
		0,
		0,
		desc.Width,
		desc.Height,
		tex,
		background->IsOnScreen(),
		background->GetOffsetX(),
		background->GetOffsetY()
	);

	return overlapSprite;
}

/*
	Clear all loaded sprites
*/
void CSprites::Clear()
{
	for (auto x : sprites)
	{
		LPSPRITE s = x.second;
		delete s;
	}

	sprites.clear();
}