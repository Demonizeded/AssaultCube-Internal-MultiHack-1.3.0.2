#pragma once
#include <Windows.h>

#include "globals.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_opengl2.h"

void initImGui() { //Initializes ImGui
	HWND gameHwnd = FindWindowA(0, "AssaultCube");
	oWndProc = (WNDPROC)SetWindowLongPtr(gameHwnd, GWLP_WNDPROC, (LONG_PTR)hkWndProc);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(gameHwnd);
	ImGui_ImplOpenGL2_Init();
}
