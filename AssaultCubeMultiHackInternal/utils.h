#pragma once
#include <cmath>

#include "Vec.h"
#include "globals.h"

typedef int(__cdecl* tSDL_SetRelativeMouseMode)(int enabled);
typedef int(__cdecl* tSDL_ShowCursor)(int toggle);

void ToggleGameMouse(bool openMenu) {
	uintptr_t baseAdd = (uintptr_t)GetModuleHandleA(NULL);
	if (baseAdd) {
		bool* isMouseGrabbed = (bool*)(baseAdd + 0x193F13);
		*isMouseGrabbed = !openMenu;
	}

	HMODULE hSDL = GetModuleHandleA("SDL2.dll");
	if (!hSDL) hSDL = GetModuleHandleA("SDL.dll");

	if (hSDL) {
		auto pSetRelativeMode = (tSDL_SetRelativeMouseMode)GetProcAddress(hSDL, "SDL_SetRelativeMouseMode");
		auto pShowCursor = (tSDL_ShowCursor)GetProcAddress(hSDL, "SDL_ShowCursor");

		if (pSetRelativeMode) {
			pSetRelativeMode(openMenu ? 0 : 1);
		}
		if (pShowCursor) {
			pShowCursor(openMenu ? 1 : 0);
		}
	}

	if (openMenu) {
		ClipCursor(NULL);
	}
}

LRESULT CALLBACK hkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_KEYDOWN && wParam == VK_INSERT && !(lParam & 0x40000000)) {
		isMenuOpen = !isMenuOpen;

		ToggleGameMouse(isMenuOpen);

		return TRUE;
	}

	if (isMenuOpen) {
		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);

		switch (uMsg) {
		case WM_LBUTTONDOWN: case WM_LBUTTONUP:
		case WM_RBUTTONDOWN: case WM_RBUTTONUP:
		case WM_MBUTTONDOWN: case WM_MBUTTONUP:
		case WM_MOUSEMOVE:   case WM_MOUSEWHEEL:
		case WM_INPUT:       case WM_SETCURSOR:
			return TRUE;
		}
	}

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

void WorldToScreen(Vec3 pos, Vec2* scr, float vm[16]) {
	Vec4 clipPos;
	clipPos.x = pos.x * vm[0] + pos.y * vm[4] + pos.z * vm[8] + vm[12];
	clipPos.y = pos.x * vm[1] + pos.y * vm[5] + pos.z * vm[9] + vm[13];
	clipPos.z = pos.x * vm[2] + pos.y * vm[6] + pos.z * vm[10] + vm[14];
	clipPos.w = pos.x * vm[3] + pos.y * vm[7] + pos.z * vm[11] + vm[15];

	if (clipPos.w < 0.2f) return;

	Vec3 NDC;
	NDC.x = clipPos.x / clipPos.w;
	NDC.y = clipPos.y / clipPos.w;
	NDC.z = clipPos.z / clipPos.w;

	scr->x = (screenWidth / 2.0f) + (NDC.x * (screenWidth / 2.0f));
	scr->y = (screenHeight / 2.0f) - (NDC.y * (screenHeight / 2.0f));
}

void drawRect(float x, float y, float boxWidth, float boxHeight, bool isFriend){
	ImDrawList* drawList =  ImGui::GetBackgroundDrawList();
	ImColor finalColor = { enemyColor[0], enemyColor[1], enemyColor[2], enemyColor[3] };
	if(isFriend) finalColor = { friendColor[0], friendColor[1], friendColor[2], friendColor[3] };
	drawList->AddRect(ImVec2(x,y), ImVec2(x + boxWidth, y + boxHeight), finalColor, 0.0f, 2.0f, 0);
}

void drawSnapLine(float x, float y, bool isFriend) {
	ImDrawList* drawList = ImGui::GetBackgroundDrawList();
	ImColor finalColor = { enemyColor[0], enemyColor[1], enemyColor[2], enemyColor[3] };
	if (isFriend) finalColor = { friendColor[0], friendColor[1], friendColor[2], friendColor[3] };
	drawList->AddLine(ImVec2(x, y), ImVec2(screenWidth / 2.0f, screenHeight - 1.0f), finalColor);
}

Vec3 getTarget() {
	float centerX = screenWidth / 2.0f;
	float centerY = screenHeight / 2.0f;

	float* vm = (float*)(baseAdd + viewMatrixOffset);
	uintptr_t entityList = *(uintptr_t*)(baseAdd + entityListOffset);
	Player* localPlayer = *(Player**)(baseAdd + localPlayerOffset);

	int numberOfPlayers = *(int*)(baseAdd + numberOfPlayersOffset);

	for (int i = 1; i < numberOfPlayers; i++) {
		Player* ent = *(Player**)(entityList + i * 4);
		if (ent->team == localPlayer->team || ent->health <= 0) continue;

		Vec2 headOnScr;

		WorldToScreen(ent->headPos, &headOnScr, vm);
		float d = std::sqrtf(std::powf(headOnScr.x - centerX, 2) + std::powf(headOnScr.y - centerY, 2));

		if (d > FOV) continue;

		return ent->headPos;
	}
	return Vec3{ 0.0f,0.0f,0.0f };
}