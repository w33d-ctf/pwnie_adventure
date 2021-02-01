#pragma once
#include <windows.h>

// VMT injecction
void* VTableHook(void* lpClass, int offset, void* hook);

// member injection
template<typename T> void HookFunction(const char *module, char *signature, T &fn_real, PVOID fn_mine);
template<typename T> void HookFunction(T &fn_real, PVOID fn_mine);
template<typename T> void HookFunction(PVOID target, T &fn_real, PVOID fn_mine);
template<typename T> void HookFunction(DWORD address, T &fn_real, PVOID fn_mine);
