#include <iostream>
#include <vector>
#include <functional>
#include <d3d11.h>
#include <dwmapi.h>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include "callbacks.h"
#include "overlay/overlay.h"
#pragma comment(lib,"d3d11.lib")


INT __fastcall WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR pStr, int cmdShow) {

	Overlay ovr = Overlay(L"Overlay Class", L"Overlay Window");

	ovr.RunLoop();

	ovr.End();

	return 0;
}