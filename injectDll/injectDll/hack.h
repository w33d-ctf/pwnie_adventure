#pragma once
#include "player.h"

typedef bool(__thiscall *RealCanJump)(Player *__this);
void __fastcall Mine_CanJumpHook(Player *__this, int edx);
