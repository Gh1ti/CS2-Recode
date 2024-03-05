#pragma once
#include "Player/Entity.h"
#include "globals.h"

#ifndef HAKS_H
#define HAKS_H
namespace hacks {

	bool W2S(const Vec& origin, Vec& screen) {
		CMatrix4x4 vm = interfaces::vm;
		float w = vm[3][0] * origin.x + vm[3][1] * origin.y + vm[3][2] * origin.z + vm[3][3];

		if (w < 0.001f)
			return false;

		const float x = origin.x * vm[0][0] + origin.y * vm[0][1] + origin.z * vm[0][2] + vm[0][3];
		const float y = origin.x * vm[1][0] + origin.y * vm[1][1] + origin.z * vm[1][2] + vm[1][3];

		w = 1.f / w;

		float nx = x * w;
		float ny = y * w;

		const ImVec2 size = ImGui::GetIO().DisplaySize;

		screen.x = (size.x * 0.5f * nx) + (nx + size.x * 0.5f);
		screen.y = -(size.y * 0.5f * ny) + (ny + size.y * 0.5f);

		return true;
	}

	void UpdateInterfaces() {
		interfaces::dwEntityList = interfaces::mem.Read<uintptr_t>(interfaces::client + offsets::dwEntityList);
		interfaces::vm = interfaces::mem.Read<CMatrix4x4>(interfaces::client + offsets::dwViewMatrix);
	}

	void esp(C_CS_PlayerPawn* pawn,ImColor color,ImDrawList* g_DrawList) {
		Vec origin = pawn->GetOrigin();
		Vec head = pawn->BoneOrigin(bones::head);
		Vec HeadWTS;
		Vec OriginWTS;
		if (pawn->GetHealth() <= 0)
			return;
		if (W2S(origin - Vec(0, 0, 7.f), OriginWTS) && W2S(head, HeadWTS)) {
			float height = HeadWTS.y - OriginWTS.y;
			float width = height * 0.5;
			ImVec2 TopLeft = ImVec2(HeadWTS.x + width / 2, HeadWTS.y);
			ImVec2 BottomRight = ImVec2(OriginWTS.x - width / 2, OriginWTS.y);

			g_DrawList->AddRect(ImVec2(TopLeft.x - 1, TopLeft.y - 1),ImVec2(BottomRight.x + 1, BottomRight.y + 1), ImColor(0,0,0,255));
			g_DrawList->AddRect(TopLeft, BottomRight, color);
			g_DrawList->AddRect(ImVec2(TopLeft.x + 1, TopLeft.y + 1), ImVec2(BottomRight.x - 1, BottomRight.y - 1), ImColor(0, 0, 0, 255));


			g_DrawList->AddLine(ImVec2(BottomRight.x + 6, BottomRight.y + 1), 
				ImVec2(BottomRight.x + 6, BottomRight.y + height - 1), ImColor(0, 0, 0, 255), 5);

			g_DrawList->AddLine(ImVec2(BottomRight.x + 6, BottomRight.y),
				ImVec2(BottomRight.x + 6, BottomRight.y + ((float)pawn->GetHealth() / 100) * height), ImColor(0, 255, 0, 255),3);

			g_DrawList->AddText(ImVec2(BottomRight.x + 12, TopLeft.y), ImColor(255, 255, 255, 255), std::to_string(pawn->GetHealth()).c_str());


		}else {
			return;
		}

	}

	void aimbot(C_CS_PlayerPawn* pawn, C_CS_PlayerPawn* lp,int lpidx,int tidx,Vec& target,Vec screen_center) {

		if (!GetAsyncKeyState(VK_LMENU))
			return;
		if (!vars::WeaponToHitBox[lp->GetWeapon()])
			return;
		W2S(pawn->BoneOrigin(vars::WeaponToHitBox[lp->GetWeapon()]), target);
		if (target.IsZero())
			return;

		if (lpidx == 0)
			return;

		if (tidx == -1)
			return;

		bool visible = pawn->spotted() & (1 << (uintptr_t)lp) || lp->spotted() & (1 << tidx);
		if (!visible)
			return;
		mouse_event(MOUSEEVENTF_MOVE, (target.x - screen_center.x) / 2, (target.y - screen_center.y) / 2, 0, 0);

	}

}
#endif // !HAKS_H
