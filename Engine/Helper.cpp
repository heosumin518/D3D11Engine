#include "pch.h"
#include "Helper.h"

wstring Engine::ToWString(const string& s)
{
	wstring wsTmp(s.begin(), s.end());
	return wsTmp;
}
