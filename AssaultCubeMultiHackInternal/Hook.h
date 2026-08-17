#pragma once
#include <Windows.h>
#include "Vec.h"

typedef BOOL(__stdcall* tWglSB)(HDC hdc);
typedef char(__fastcall* tHkShoot)(void* weaponClass, void* edx, Vec3* targetVector);
typedef int(__fastcall* tHkDamage)(void* plr, void* edx, int dmg, int weaponID);

tWglSB vWglSB = nullptr;
tHkShoot vHkShoot = nullptr;
tHkDamage vHkDamage = nullptr;