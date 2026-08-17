#pragma once
#include "Hook.h"
#include "render.h"
#include "init.h"
#include "trampoline.h"
#include "cheats.h"

//additional thread to handle Bhop and find the best player for silentAim
DWORD WINAPI miscThread() {
	while (true) {
		Sleep(5);
		chosenTarget = getTarget();
		Player* localPlayer = *(Player**)(baseAdd + localPlayerOffset);
		if (isBunnyHopOn && localPlayer) bunnyHop(localPlayer);
	}
}

DWORD WINAPI start() {
	HMODULE oGL = GetModuleHandleA("opengl32.dll");
	uintptr_t gRTarget = (uintptr_t)GetProcAddress(oGL, "wglSwapBuffers"); //We retrieve the address of the wglSwapBuffers function from the opengl32 module for the hook in order to draw our Menu,ESP and FOV 
	uintptr_t silentTarget = (uintptr_t)(baseAdd + shootFuncOffset); //Shoot func address
	uintptr_t instaKillTarget = (uintptr_t)(baseAdd + damageFuncOffset); //Damage func address

	initImGui();

	//Hooks
	vWglSB = reinterpret_cast<tWglSB>(Trampoline((BYTE*)gRTarget, (BYTE*)guiHook, 5));
	vHkShoot = reinterpret_cast<tHkShoot>(Trampoline((BYTE*)silentTarget, (BYTE*)silentAimHook, 10));
	vHkDamage = reinterpret_cast<tHkDamage>(Trampoline((BYTE*)instaKillTarget, (BYTE*)instaKillHook, 5));

	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)miscThread, 0, NULL, 0);

	return 1;
}
