#include "overlay.h"
#include "../SDK/haks.h"
#include <iostream>
#ifdef OVERLAY_H

Overlay::Overlay(const wchar_t* class_name, const wchar_t* window_name) {

	wc.cbSize = sizeof(WNDCLASSEXW);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = Callbacks::WindowProcedure;
	wc.hInstance = GetModuleHandle(nullptr);
	wc.lpszClassName = class_name;

	RegisterClassExW(&wc);

	window = CreateWindowExW(
		WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED,
		wc.lpszClassName,
		window_name,
		WS_POPUP,
		0,
		0,
		GetSystemMetrics(SM_CXFULLSCREEN),
		GetSystemMetrics(SM_CYFULLSCREEN),
		nullptr,
		nullptr,
		wc.hInstance,
		nullptr
	);
	SetLayeredWindowAttributes(window, RGB(0, 0, 0), BYTE(255), LWA_ALPHA);

	{
		RECT* client_area = new RECT;
		GetClientRect(window, client_area);

		RECT* window_area = new RECT;
		GetWindowRect(window, window_area);

		POINT* diff = new POINT;
		ClientToScreen(window, diff);

		const MARGINS* margins = new MARGINS{
			window_area->left + (diff->x - window_area->left),
			window_area->top + (diff->y - window_area->top),
			client_area->right,
			client_area->bottom
		};

		DwmExtendFrameIntoClientArea(window, margins);

		delete client_area;
		delete window_area;
		delete diff;
	}
	sd.BufferDesc.RefreshRate.Numerator = 144U;
	sd.BufferDesc.RefreshRate.Denominator = 1U;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.SampleDesc.Count = 1U;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1U;
	sd.OutputWindow = window;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	constexpr D3D_FEATURE_LEVEL levels[2]{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0
	};

	D3D11CreateDeviceAndSwapChain(nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0U,
		levels,
		2U,
		D3D11_SDK_VERSION,
		&sd,
		&g_pswapChain,
		&g_pdDevice,
		&level,
		&p_pd3dDeviceContext);

	ID3D11Texture2D* backBuffer;
	g_pswapChain->GetBuffer(0U, IID_PPV_ARGS(&backBuffer));

	if (backBuffer) {
		g_pdDevice->CreateRenderTargetView(backBuffer, nullptr, &g_pdRenderTargetView);
		backBuffer->Release();
	}
	else {
		std::cerr << "BackBuffer was null \n";
	}

	ShowWindow(window, 1);
	UpdateWindow(window);

	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(g_pdDevice, p_pd3dDeviceContext);
	ImGui::GetIO().IniFilename = nullptr;
	running = true;
	//AllocConsole();
#pragma warning(disable: 4996)
	//freopen("CONOUT$", "w", stdout);
#pragma warning(default: 4996)
	//std::cout << "Alo \n";
}

void Overlay::RunLoop() {
	while (running) {
		MSG msg;
		while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT) {
				running = false;
			}
			if (!running)
				break;

		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();

		ImGui::NewFrame();
		Vec screen_center = Vec(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2);
		HWND cs2 = FindWindow(NULL, "Counter-Strike 2");
		if (!cs2)
			break;
		HWND currentWindow = GetForegroundWindow();

		if (currentWindow == cs2) {
			hacks::UpdateInterfaces();
			float fov = 20;
			Vec target = Vec(0,0);
			int LocalIdx = 0;
			int TargetIndex = -1;
			ImDrawList* g_DrawList = ImGui::GetBackgroundDrawList();
			CBaseEntity* LocalController = CBaseEntity::GetLocalPlayer();
			C_CS_PlayerPawn* LocalPlayer = C_CS_PlayerPawn::GetLocalPawn();
			C_CS_PlayerPawn* targetPlayer = nullptr;
			int localTeam = LocalController->GetTeam();
			for (int i = 1; i < 64; i++) {
				CBaseEntity* ent = CBaseEntity::GetEntity(i);
				if (!ent)
					continue;

				C_CS_PlayerPawn* player = C_CS_PlayerPawn::GetPawn(ent);

				if (player == LocalPlayer) {
					LocalIdx = i;
					continue;
				}

				if (ent->GetTeam() == LocalController->GetTeam())
					continue;

				if (GetAsyncKeyState(VK_RMENU) & 1)
					vars::espEnabled = !vars::espEnabled;

				if (vars::espEnabled) {
					hacks::esp(player, ImColor(255, 255, 255, 255), g_DrawList);
				}

				if (LocalPlayer->GetHealth() <= 0)
					continue;

				if (player->GetHealth() <= 0)
					continue;

				Vec world_head = player->BoneOrigin(bones::head);
				Vec screenHead;
				if (hacks::W2S(world_head, screenHead)) {
					float dist = screen_center.Dist(screenHead);
					if (dist < fov) {
						fov = dist;
						target = screenHead;
						targetPlayer = player;
						TargetIndex = i;
					}
				}
			}
			hacks::aimbot(targetPlayer, LocalPlayer, LocalIdx, TargetIndex, target, screen_center);
		}
		ImGui::Render();

		constexpr float color[4] = { 0.f,0.f,0.f,0.f };
		p_pd3dDeviceContext->OMSetRenderTargets(1U, &g_pdRenderTargetView, nullptr);
		p_pd3dDeviceContext->ClearRenderTargetView(g_pdRenderTargetView, color);

		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		g_pswapChain->Present(1U, 0U);
	}
}

void Overlay::End() {
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	if (g_pdDevice)
		g_pdDevice->Release();

	if (p_pd3dDeviceContext)
		p_pd3dDeviceContext->Release();

	if (g_pswapChain)
		g_pswapChain->Release();

	if (g_pdRenderTargetView)
		g_pdRenderTargetView->Release();

	DestroyWindow(window);
	UnregisterClassW(wc.lpszClassName, wc.hInstance);
};


#endif;