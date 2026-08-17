#pragma once

#include <windows.h>

//trampoline function to put EIP back on track after our inline hook

BYTE* Trampoline(BYTE* src, BYTE* dst, const uintptr_t len) {
	BYTE* gateway = (BYTE*)VirtualAlloc(0, len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	memcpy(gateway, src, len);

	uintptr_t gRelAdd = (uintptr_t)src - (uintptr_t)gateway - 5;

	*(gateway + len) = 0xE9;
	*(uintptr_t*)((uintptr_t)gateway + len + 1) = gRelAdd;

	DWORD oldProt;

	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &oldProt);

	uintptr_t relAdd = (uintptr_t)dst - (uintptr_t)src - 5;

	*(src) = 0xE9;
	*(uintptr_t*)((uintptr_t)src + 1) = relAdd;
	for (uintptr_t i = 5; i < len; i++) *(src + i) = 0x90;

	VirtualProtect(src, len, oldProt, &oldProt);

	return gateway;
}