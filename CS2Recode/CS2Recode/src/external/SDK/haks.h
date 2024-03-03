#pragma once
#include "Player/Entity.h"

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

	void esp() {
		CBaseEntity* lp = CBaseEntity::GetLocalPlayer();
		C_CS_PlayerPawn* lp_pawn = C_CS_PlayerPawn::GetLocalPawn();
		for (int i = 0; i < 64; i++) {
			CBaseEntity* player = CBaseEntity::GetEntity(i);
			C_CS_PlayerPawn* player_pawn = C_CS_PlayerPawn::GetPawn(player);

			if (player_pawn->GetHealth() <= 0)
				continue;

			if (player->GetTeam() == lp->GetTeam())
				continue;

			Vec world_feet = player_pawn->GetOrigin();
			Vec world_head = player_pawn->BoneOrigin(bones::head);
			Vec screenFeet;
			Vec screenHead;
			if (W2S(world_feet - Vec(0, 0, 7.f), screenFeet) && W2S(world_head, screenHead)) {
				float height = screenHead.y - screenFeet.y;
				float width = height * 0.30;

				ImVec2 TopLeft = ImVec2(screenHead.x - width, screenHead.y);
				ImVec2 BottomRight = ImVec2(screenFeet.x + width, screenFeet.y);

				ImGui::GetBackgroundDrawList()->AddRect(TopLeft, BottomRight, ImColor(255, 255, 255, 255));

			}
		}
	}

	void aimbot() {
		if (!GetAsyncKeyState(VK_LMENU))
			return;

		C_CS_PlayerPawn* lp = C_CS_PlayerPawn::GetLocalPawn();

		if ((uintptr_t)lp == 0)
			return;

		if (lp->GetHealth() <= 0)
			return;

		float fov = 50;
		Vec target;
		int team = CBaseEntity::GetLocalPlayer()->GetTeam();
		int lpidx;
		C_CS_PlayerPawn* targerPlayer = nullptr;
		Vec screen_center = Vec(ImGui::GetIO().DisplaySize.x/2, ImGui::GetIO().DisplaySize.y / 2);

		for (int i = 1; i < 64; i++) {
			CBaseEntity* controller = CBaseEntity::GetEntity(i);

			if ((uintptr_t)controller == 0)
				continue;
			if (controller->GetTeam() == team)
				continue;
			C_CS_PlayerPawn* pawn = C_CS_PlayerPawn::GetPawn(controller);

			if (pawn->GetHealth() <= 0)
				continue;

			if (pawn == lp) {
				lpidx = i;
				continue;
			}

			Vec world_head = pawn->BoneOrigin(bones::head);
			Vec screenHead;
			if (W2S(world_head, screenHead)) {
				float dist = screen_center.Dist(screenHead);
				if (dist < fov) {
					fov = dist;
					target = screenHead;
					targerPlayer = pawn;
				}
			}
		}

		if (target.IsZero())
			return;

		if (lp->GetWeapon() != "weapon_awp")
			return;

		bool visible = targerPlayer->GetSpottedByMask() & (1 << (uintptr_t)lp);
		if (!visible)
			return;
		mouse_event(MOUSEEVENTF_MOVE, (target.x - screen_center.x)/2, (target.y - screen_center.y)/2, 0, 0);

	};
}
#endif // !HAKS_H
