#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include <d3d11.h>
#include <dwmapi.h>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include "../callbacks.h"

#ifndef OVERLAY_H
#define OVERLAY_H

class Overlay {
private:
	WNDCLASSEXW wc;
	HWND window;
	DXGI_SWAP_CHAIN_DESC    sd;
	ID3D11Device*           g_pdDevice;
	ID3D11DeviceContext*    p_pd3dDeviceContext;
	IDXGISwapChain*         g_pswapChain;
	ID3D11RenderTargetView* g_pdRenderTargetView;
	D3D_FEATURE_LEVEL       level;
	ID3D11Texture2D*        backBuffer;
	bool running;
public:
	Overlay(const wchar_t* class_name, const wchar_t* window_name);

	void RunLoop();

	void End();
};

#endif
