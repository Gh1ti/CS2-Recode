#pragma once
#include <cstdint>

namespace offsets {
	constexpr std::uintptr_t dwEntityList = 0x18BBAF8;
	constexpr std::uintptr_t dwLocalPlayerController = 0x190B308;
	constexpr std::uintptr_t dwLocalPlayerPawn = 0x1730118;
	constexpr std::uintptr_t dwViewAngles = 0x1929730;
	constexpr std::uintptr_t dwViewMatrix = 0x191CF30;
	constexpr std::uintptr_t m_iHealth = 0x334;
	constexpr std::uintptr_t m_iTeamNum = 0x3CB;
	constexpr std::uintptr_t m_vOldOrigin = 0x127C;
	constexpr std::uintptr_t m_pGameSceneNode = 0x318;
	constexpr std::uintptr_t m_modelState = 0x160;
	constexpr std::uintptr_t m_boneArray = 0x80;
	constexpr std::uintptr_t m_hPlayerPawn = 0x7E4;
	constexpr std::uintptr_t m_entitySpottedState = 0x1698;
	constexpr std::uintptr_t m_bSpottedByMask = 0xC;
	constexpr std::ptrdiff_t m_pClippingWeapon = 0x1308;
	constexpr std::ptrdiff_t m_pEntity = 0x10;
	constexpr std::ptrdiff_t m_designerName = 0x20;
}