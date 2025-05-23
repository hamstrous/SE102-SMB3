#pragma once

#include <Windows.h>

#include "GameObject.h"

#include <signal.h>
#include <string>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <vector>

class CGameObject;

using namespace std;

vector<string> split(string line, string delimeter = "\t");
wstring ToWSTR(string st);

LPCWSTR ToLPCWSTR(string st);

void Clamp(float& x, float min, float max);

void ToUpperCase(std::string& str);

void DebugObjectType(LPGAMEOBJECT obj);

extern bool debug;
extern DWORD DT;
