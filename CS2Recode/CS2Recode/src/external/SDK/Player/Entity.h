#pragma once
#include "../interfaces.h"
#include "../Vector.h"
struct CBaseEntity {
	static CBaseEntity* GetEntity(int i) {
		uintptr_t list_entry = interfaces::mem.Read<uintptr_t>(interfaces::dwEntityList + 0x10);
		return interfaces::mem.Read<CBaseEntity*>(list_entry + 120 * i);
	};

	static CBaseEntity* GetLocalPlayer() {
		return interfaces::mem.Read<CBaseEntity*>(interfaces::client + offsets::dwLocalPlayerController);
	};

	int GetTeam() {
		return interfaces::mem.Read<int>(reinterpret_cast<uintptr_t>(this) + offsets::m_iTeamNum);
	};
};

struct C_CS_PlayerPawn {
	static C_CS_PlayerPawn* GetPawn(CBaseEntity* entity) {
		uintptr_t uHandle = interfaces::mem.Read<uintptr_t>(reinterpret_cast<uintptr_t>(entity) + offsets::m_hPlayerPawn);
		uintptr_t uListEntry = interfaces::mem.Read<uintptr_t>(interfaces::dwEntityList+ 0x8 * ((uHandle & 0x7FFF) >> 9) + 16);
		return interfaces::mem.Read<C_CS_PlayerPawn*>(uListEntry + 120 * (uHandle & 0x1FF));
	};
	

	static C_CS_PlayerPawn* GetLocalPawn() {
		return interfaces::mem.Read<C_CS_PlayerPawn*>(interfaces::client + offsets::dwLocalPlayerPawn);
	};

	int GetHealth() {
		return interfaces::mem.Read<int>(reinterpret_cast<uintptr_t>(this) + offsets::m_iHealth);
	};

	Vec GetOrigin() {
		return interfaces::mem.Read<Vec>(reinterpret_cast<uintptr_t>(this) + offsets::m_vOldOrigin);
	};

	Vec BoneOrigin(int index) {
		uintptr_t gamescene = interfaces::mem.Read<uintptr_t>(reinterpret_cast<uintptr_t>(this) + offsets::m_pGameSceneNode);
		uintptr_t bonearray = interfaces::mem.Read<uintptr_t>(gamescene + offsets::m_modelState + offsets::m_boneArray);
		Vec position = interfaces::mem.Read<Vec>(bonearray + index * 32);
		return position;
	};

	uintptr_t GetSpottedByMask() {
		return interfaces::mem.Read<uintptr_t>(uintptr_t(this) + offsets::m_entitySpottedState + offsets::m_bSpottedByMask);
	}

	std::string GetWeapon() {
		uintptr_t weapon_base = interfaces::mem.Read<uintptr_t>((uintptr_t)this + offsets::m_pClippingWeapon);
		uintptr_t p_entity = interfaces::mem.Read<uintptr_t>(weapon_base + offsets::m_pEntity);
		uintptr_t weapon_name = interfaces::mem.Read<uintptr_t>(p_entity + offsets::m_designerName);
		return interfaces::mem.ReadString(weapon_name, 256);
	}


};

enum bones : int
{
	head = 6,
	neck = 5,
	spine = 4,
	spine_1 = 2,
	left_shoulder = 8,
	left_arm = 9,
	left_hand = 11,
	cock = 0,
	right_shoulder = 13,
	right_arm = 14,
	right_hand = 16,
	left_hip = 22,
	left_knee = 23,
	left_feet = 24,
	right_hip = 25,
	right_knee = 26,
	right_feet = 27
};