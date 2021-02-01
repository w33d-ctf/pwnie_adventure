#include "framework.h"
#include "detours.h"
#include "hack.h"

template<typename T>
void HookFunction(const char *module, char *signature, T &fn_real, PVOID fn_mine)
{
	HookFunction<T>(DetourFindFunction(module, signature), fn_real, fn_mine);
}
template<typename T>
void HookFunction(T &fn_real, PVOID fn_mine)
{
	DetourAttach(&(PVOID&)fn_real, fn_mine);
}
template<typename T>
void HookFunction(PVOID target, T &fn_real, PVOID fn_mine)
{
	fn_real = reinterpret_cast<T>(target);

	HookFunction<T>(fn_real, fn_mine);
}

template<typename T>
void HookFunction(DWORD address, T &fn_real, PVOID fn_mine)
{
	HookFunction<T>(reinterpret_cast<PVOID>(address), fn_real, fn_mine);
}

extern RealCanJump oriCanJump;

void ApplyHooks()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	HookFunction<RealCanJump>(0x10051680, oriCanJump, Mine_CanJumpHook);

	DetourTransactionCommit();
}