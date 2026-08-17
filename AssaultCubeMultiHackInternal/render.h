#pragma once

#include "player.h"
#include "offsets.h"
#include "utils.h"
#include "cheats.h"
#include "Hook.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_opengl2.h"


BOOL __stdcall guiHook(HDC hdc) {
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGuiIO& io = ImGui::GetIO();
	io.MouseDrawCursor = isMenuOpen;

	float* vm = (float*)(baseAdd + viewMatrixOffset);

	Player* localPlayer = *(Player**)(baseAdd + localPlayerOffset);
	
	uintptr_t entityList = *(uintptr_t*)(baseAdd + entityListOffset);

	int numberOfPlayers = *(int*)(baseAdd + numberOfPlayersOffset);

	for (int i = 1; i < numberOfPlayers; ++i) {
		Player* entity = *(Player**)(entityList + i * 4);
		if (entity->health < 1) continue;

		Vec2 foots, head;
		
		WorldToScreen(entity->pos, &foots, vm);
		WorldToScreen(entity->headPos, &head, vm);
		if(isESPOn) ESP(head,foots, localPlayer->team == entity->team);
		if (isFOVOn) drawFov();
	}

	if (isMenuOpen) {
		ImGui::Begin("Internal AssaultCube MultiHack made by Demonized");
		ImGui::SetWindowSize(ImVec2(450, 300));
		if (ImGui::CollapsingHeader("ESP Settings")) {
			ImGui::Checkbox("ESP", &isESPOn);
			ImGui::Checkbox("SnapLines", &isSnapLineOn);
			ImGui::ColorEdit4("Friend's Color", friendColor);
			ImGui::ColorEdit4("Enemy's Color", enemyColor);
		}
		if (ImGui::CollapsingHeader("SilentAim Settings")) {
			ImGui::Checkbox("SilentAim", &isSilentAimOn);
			ImGui::Checkbox("Silent Aim FOV", &isFOVOn);
			ImGui::SliderFloat("FOV Size", &FOV, 3.0f, 2000.0f);
			ImGui::ColorEdit4("FOV Color", fovColor);
		}
		if (ImGui::CollapsingHeader("InstaKill Settings")) {
			ImGui::Checkbox("InstaKill", &isInstaKillOn);
		}
		if (ImGui::CollapsingHeader("BunnyHop Settings")) {
			ImGui::Checkbox("BunnyHop", &isBunnyHopOn);
			ImGui::Checkbox("BunnyHop SpeedHack", &isBunnyHopSpeedOn);
			ImGui::SliderFloat("BunnyHop Speed", &bunnyHopSpeed, 0.4f, 30.0f);
		}
		ImGui::End();
	}
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	return vWglSB(hdc);
}