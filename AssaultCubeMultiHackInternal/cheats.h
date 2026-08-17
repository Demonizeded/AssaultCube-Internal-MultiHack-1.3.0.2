#pragma once
#include "utils.h"


void ESP(Vec2 head, Vec2 screen, bool isFriend) {
	float boxHeight = screen.y - head.y;
	float boxWidth = boxHeight / 2.0f;

	drawRect(head.x - (boxWidth / 2.0f), head.y, boxWidth, boxHeight, isFriend);
	if(isSnapLineOn)drawSnapLine(screen.x , screen.y, isFriend);
}

//Intercepts the engine's shoot function to redirect the projectile vector directly to enemy's head without altering player's camera
char __fastcall silentAimHook(void* weaponClass, void* edx, Vec3* target) {
	Vec3 bestDest = chosenTarget;

	if (!isSilentAimOn || (bestDest.x == 0.0f && bestDest.y == 0.0f)) return vHkShoot(weaponClass, edx, target);
	return vHkShoot(weaponClass, edx, &bestDest);
}

//Intercepts the damage dispatch function to force lethal outgoing damage values.
int __fastcall instaKillHook(void* plr, void* edx, int dmg, int weaponID) {
	if(!isInstaKillOn) return vHkDamage(plr, edx, dmg, weaponID);
	uintptr_t localPlayer = (uintptr_t)(baseAdd + localPlayerOffset);
	if (localPlayer == (uintptr_t)plr) return vHkDamage(plr, edx, dmg, weaponID);

	return vHkDamage(plr, edx, 300, weaponID);
}

void drawFov() {
	ImDrawList* drawList = ImGui::GetBackgroundDrawList();
	drawList->AddCircle(ImVec2(screenWidth / 2.0f, screenHeight / 2.0f), FOV, ImColor(fovColor[0], fovColor[1], fovColor[2], fovColor[3]), 36);
}

//Handles BHop(Converts YAW to radians taking into account the Cube engine's inverted Y-axis convention)
void bunnyHop(Player* plr) {
	if ((GetAsyncKeyState(VK_SPACE) & 0x8000) && plr->onGround) {

		plr->vel.z += 1.0f;
		
		if (isBunnyHopSpeedOn) {
			float rad = plr->yaw * (3.14159265f / 180.0f);

			float dirX = std::sin(rad);
			float dirY = -std::cos(rad);

			plr->vel.x = dirX * bunnyHopSpeed;
			plr->vel.y = dirY * bunnyHopSpeed;
		}
	}
}