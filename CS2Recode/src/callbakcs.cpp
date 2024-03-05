#include "callbacks.h"

LRESULT __fastcall Callbacks::WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) {
		return 0L;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}