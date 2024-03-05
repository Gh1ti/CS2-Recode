#pragma once
#include <d3d11.h>
#include <dwmapi.h>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class Callbacks {
public:
	static LRESULT __stdcall WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};