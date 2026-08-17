#pragma once
#include <Windows.h>

uintptr_t baseAdd = (uintptr_t)GetModuleHandle(NULL); 

uintptr_t localPlayerOffset = 0x17E0A8; //Our Player Offset
uintptr_t viewMatrixOffset = 0x17DFD0; //Viewmatrix offset
uintptr_t entityListOffset = 0x18AC04; //List of Entities on the server
uintptr_t numberOfPlayersOffset = 0x18AC0C; //Number of Players on the Server

uintptr_t shootFuncOffset = 0xC7200; //Shoot function offset so we can inline hook and implement our silentAim
uintptr_t damageFuncOffset = 0x1C130; //Damage function so we can hook our Insta Kill
