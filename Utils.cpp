#include <Windows.h>
#include <algorithm>
#include <cctype>


#include "PlayScene.h"
#include "Camera.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Coin.h"
#include "Platform.h"
#include "Cloud.h"
#include "Floor.h"
#include "Pipe.h"
#include "Fireball.h"
#include "Plant.h"
#include "KoopaRed.h"
#include "KoopaGreen.h"
#include "ColorBlock.h"
#include "Koopa.h"
#include "QuestionBlock.h"
#include "Mushroom.h"
#include "Leaf.h"
#include "Mountain.h"
#include "Smoke.h"
#include "SampleKeyEventHandler.h"
#include "BackgroundColor.h"
#include "UnbreakableBrick.h"
#include "GenericPlatform.h"
#include "PowerUp.h"
#include "Score.h"
#include "Font.h"
#include "GameFX.h"
#include "CloudPlatform.h"
#include "GameFXManager.h"
#include "GameFX.h"
#include "TimerManager.h"
#include "Abyss.h"


#include "Utils.h"

vector<string> split(string line, string delimeter)
{
	vector<string> tokens;
	size_t last = 0; size_t next = 0;
	while ((next = line.find(delimeter, last)) != string::npos)
	{
		tokens.push_back(line.substr(last, next - last));
		last = next + 1;
	}
	tokens.push_back(line.substr(last));

	return tokens;
}

/*
char * string to wchar_t* string.
*/
wstring ToWSTR(string st)
{
	const char *str = st.c_str();

	size_t newsize = strlen(str) + 1;
	wchar_t * wcstring = new wchar_t[newsize];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, str, _TRUNCATE);

	wstring wstr(wcstring);
	// delete wcstring   // << can I ? 
	delete[] wcstring; // << I can ?

	return wstr;
}

/*
	Convert char* string to wchar_t* string.
*/
LPCWSTR ToLPCWSTR(string st)
{
	size_t newsize = st.length() + 1;
	wchar_t* wcstring = new wchar_t[newsize];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, st.c_str(), _TRUNCATE);
	return wcstring;// << I can return it, but I should delete it later
}

void Clamp(float& x, float min, float max)
{
	if (x < min)
		x = min;
	if (x > max)
		x = max;
}

void ToUpperCase(std::string& str)
{
	for (char& c : str) {
		c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
	}
}

void DebugObjectType(LPGAMEOBJECT obj)
{
	if(dynamic_cast<CPortal*>(obj))
	{
		DebugOut(L"[DEBUG] Portal\n");
	}
	else if (dynamic_cast<CCoin*>(obj))
	{
		DebugOut(L"[DEBUG] Coin\n");
	}
	else if (dynamic_cast<CPlatform*>(obj))
	{
		DebugOut(L"[DEBUG] Platform\n");
	}
	else if (dynamic_cast<CCloud*>(obj))
	{
		DebugOut(L"[DEBUG] Cloud\n");
	}
	else if (dynamic_cast<CFloor*>(obj))
	{
		DebugOut(L"[DEBUG] Floor\n");
	}
	else if (dynamic_cast<CPipe*>(obj))
	{
		DebugOut(L"[DEBUG] Pipe\n");
	}
	else if (dynamic_cast<CFireball*>(obj))
	{
		DebugOut(L"[DEBUG] Fireball\n");
	}
	else if (dynamic_cast<CPlant*>(obj))
	{
		DebugOut(L"[DEBUG] Plant\n");
	}
	else if (dynamic_cast<CKoopaRed*>(obj))
	{
		DebugOut(L"[DEBUG] KoopaRed\n");
	}
	else if (dynamic_cast<CKoopaGreen*>(obj))
	{
		DebugOut(L"[DEBUG] KoopaGreen\n");
	}
	else if (dynamic_cast<CColorBlock*>(obj))
	{
		DebugOut(L"[DEBUG] ColorBlock\n");
	}
	else if (dynamic_cast<CKoopa*>(obj))
	{
		DebugOut(L"[DEBUG] Koopa\n");
	}
	else if (dynamic_cast<CQuestionBlock*>(obj))
	{
		DebugOut(L"[DEBUG] QuestionBlock\n");
	}
	else if (dynamic_cast<CMushroom*>(obj))
	{
		DebugOut(L"[DEBUG] Mushroom\n");
	}
	else if (dynamic_cast<CLeaf*>(obj))
	{
		DebugOut(L"[DEBUG] Leaf\n");
	}
	else if (dynamic_cast<CMountain*>(obj))
	{
		DebugOut(L"[DEBUG] Mountain\n");
	}
	else if (dynamic_cast<CSmoke*>(obj))
	{
		DebugOut(L"[DEBUG] Smoke\n");
	}else if(dynamic_cast<CMario*>(obj))
	{
		DebugOut(L"[DEBUG] Mario\n");
	}
	else if (dynamic_cast<CBackgroundColor*>(obj))
	{
		DebugOut(L"[DEBUG] BackgroundColor\n");
	}
	else if (dynamic_cast<CUnbreakableBrick*>(obj))
	{
		DebugOut(L"[DEBUG] UnbreakableBrick\n");
	}
	else if (dynamic_cast<CGenericPlatform*>(obj))
	{
		DebugOut(L"[DEBUG] GenericPlatform\n");
	}
	else if (dynamic_cast<CPowerUp*>(obj))
	{
		DebugOut(L"[DEBUG] PowerUp\n");
	}
	else if (dynamic_cast<CScore*>(obj))
	{
		DebugOut(L"[DEBUG] Score\n");
	}
	else if (dynamic_cast<CFont*>(obj))
	{
		DebugOut(L"[DEBUG] Font\n");
	}
	else if (dynamic_cast<CAbyss*>(obj))
	{
		DebugOut(L"[DEBUG] Abyss\n");
	}
	else
	{
		DebugOut(L"[DEBUG] Unknown object type: %d \n");
	}
}

bool debug = false;
DWORD DT = 0;