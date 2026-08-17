#pragma once
#include "utils.h"

#include "imgui/imgui.h"

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int screenWidth = GetSystemMetrics(SM_CXSCREEN);;
int screenHeight = GetSystemMetrics(SM_CYSCREEN);

bool isMenuOpen = false;

bool isInstaKillOn = false;

bool isBunnyHopOn = false;
bool isBunnyHopSpeedOn = false;

bool isSnapLineOn = false;
bool isESPOn = false;

bool isSilentAimOn = false;
bool isFOVOn = false;

float FOV = 40.0f;
float bunnyHopSpeed = 1.0f;

float enemyColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
float friendColor[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
float fovColor[4] = { 0.0f, 1.0f, 0.0f, 1.0f };

WNDPROC oWndProc = NULL;

Vec3 chosenTarget;