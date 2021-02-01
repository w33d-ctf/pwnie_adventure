#include "hack.h"

RealCanJump oriCanJump;
void __fastcall Mine_CanJumpHook(Player*__this, int edx)
{
	/*
		[NOTE]: __this default with offset 0x70 due to IActor
	*/
	auto x = *(float *)((DWORD)__this - 0x70 + 0x44 + 0x00);
	auto y = *(float *)((DWORD)__this - 0x70 + 0x44 + 0x04);
	auto z = *(float *)((DWORD)__this - 0x70 + 0x44 + 0x08);

	std::string* playerName = (std::string *)((DWORD)__this - 0x70 + 0x78);
	
	char strBuf[1000];
	int len = sprintf_s(strBuf, "[DLL](%f, %f, %f)", x, y, z);
	OutputDebugStringW(std::wstring(strBuf, strBuf + len).c_str());

	len = sprintf_s(strBuf, "[DLL]playerName:%s", playerName->c_str());
	OutputDebugStringW(std::wstring(strBuf, strBuf + len).c_str());
	
	int mana = *(int *)((DWORD)__this - 0x70 + 0x12c);
	len = sprintf_s(strBuf, "[DLL]mana:%d", mana);
	OutputDebugStringW(std::wstring(strBuf, strBuf + len).c_str());

	oriCanJump(__this);
}
/*
char __fastcall Mine_UseMana(Player *this, int mana)
{

}
*/