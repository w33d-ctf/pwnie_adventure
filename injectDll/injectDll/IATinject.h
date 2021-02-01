#pragma once
#include <iostream>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <unordered_map>
#include "framework.h"

/*
	example:

	typedef int (WINAPI *TMesBoxFptr)(HWND, LPCWSTR, LPCWSTR, UINT);
	TMesBoxFptr pMessageBox = NULL;

	__declspec(dllexport)
	int
	WINAPI
	MyMessageBoxW(
		_In_opt_ HWND hWnd,
		_In_opt_ LPCWSTR lpText,
		_In_opt_ LPCWSTR lpCaption,
		_In_ UINT uType)
	{
		if (pMessageBox == NULL)
		{
			perror("empty ptr of msg box");
			return -1;
		}
		pMessageBox(NULL, L"executing hacked", L"executing hacked", MB_ICONWARNING);
		return pMessageBox(hWnd, L"hacked", lpCaption, uType);
	}

	IATtargetFunc user32Targets[] = {
		{(DWORD_PTR*)(&pMessageBox), (DWORD_PTR)MyMessageBoxW, "MessageBoxW"},
	};

	IATtargetLib IATtargets[] = {
		{
			"USER32.dll", user32Targets, sizeof(user32Targets) / sizeof(IATtargetFunc)
		},
	};
	int IATtargetsCount = sizeof(IATtargets) / sizeof(IATtargetLib);
*/


struct IATtargetFunc
{
	DWORD_PTR *pOriFuc;
	DWORD_PTR pNewFuc;

	std::string fuc_name;
};
struct IATtargetLib
{
	std::string libname;
	IATtargetFunc *targetFuncs;
	int targetFuncsCount;
};
bool LoadLibPtr();
void IAT_Hijacking();