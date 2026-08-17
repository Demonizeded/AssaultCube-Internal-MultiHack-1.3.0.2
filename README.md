# AssaultCube Internal 1.3.0.2 Made By Demonized

A modular, internal C++ framework and runtime modification suite for the Cube Engine (AssaultCube v1.3.0.2). This project demonstrates low-level systems programming concepts including custom x86 inline trampoline hooking, OpenGL graphics pipeline interception, matrix mathematics for 3D-to-2D screen projection, and direct engine memory reconstruction.
---
> [!WARNING]  
> ** This software is intended solely for offline analysis, single-player environments, and private educational research. **Do not use this tool in public multiplayer sessions or ruin the experience for other players.****
---

## 🛠️ Technical Highlights

* **Custom x86 Trampoline Hook Engine:** Implements runtime byte manipulation, relative jump offset calculation (`E9` JMP rel32), stolen-byte gateway allocation (`VirtualAlloc`), and memory page protection management (`VirtualProtect`).
* **Hardware-Accelerated Rendering Pipeline Interception:** Intercepts `wglSwapBuffers` to embed a Dear ImGui overlay directly into the game's active OpenGL context before frame presentation.
* **Math & Linear Algebra Pipeline:** Custom implementation of 3D-to-2D `WorldToScreen` transformation using the engine's 4x4 ViewMatrix, homogenous coordinate clipping ($W < 0.2$), and Normalized Device Coordinates (NDC) mapping.
* **Low-Level Input Subversion:** Hooks window procedure (`WndProc`) and dynamically interfaces with `SDL.dll` / `SDL2.dll` (`SDL_SetRelativeMouseMode`, `SDL_ShowCursor`) to release mouse confinement when the GUI is active.
* **Asynchronous Multi-Threading:** Offloads computationally intensive operations (continuous target validation, input polling, velocity vector recalculation) to an independent worker thread.

---

## 🚀 Core Features

### Visual Engine (ESP & HUD) - Using ImGui
* **2D Bounding Boxes:** Dynamic spatial bounding boxes calculated from 3D world coordinates.
* **Snaplines:** Bottom-to-center trajectory lines directed at active targets.
* **Team & Entity Filtering:** Differentiates between friendly and hostile entities across both Team and Free-For-All (FFA) game modes.
* **FOV Visualizer:** Dynamic screen-space circle representing the valid targeting radius.

### Combat Mechanics
* **Silent Aim:** Intercepts the native projectile dispatch routine (`__fastcall`) to redirect bullet trajectories toward the target vector without overriding the local player's visual pitch/yaw camera angles.
* **InstaKill:** Hooks damage dispatch routines (`__fastcall`) to dynamically override damage payloads for non-local entities.

### Movement
* **Vector-Based Strafe Bunnyhop:** Bypasses engine jump deceleration by monitoring ground collision flags (`0x5F`), resetting internal jump states, and applying directional velocity impulses calculated using trigonometry on the Cube engine's inverted Y-axis.

---

## Project Architecture

```text
├── core.h          # Orchestration layer, thread initialization, and hook deployment
├── trampoline.h    # Low-level x86 trampoline gateway & memory patching engine
├── Hook.h          # Function pointer definitions and calling convention signatures
├── offsets.h       # Reverse-engineered memory addresses and data structures
├── player.h        # Byte-aligned reconstruction of the engine's Player class
├── utils.h         # Linear algebra (W2S), drawing routines, and SDL input handling
├── render.h        # OpenGL wglSwapBuffers detour & ImGui rendering loop
├── cheats.h        # Core logic: SilentAim hook, InstaKill detour, and Bunnyhop physics
├── init.h          # Window procedure hooking and ImGui context initialization
└── dllmain.cpp     # Dynamic-Link Library entry point (DLL_PROCESS_ATTACH)
