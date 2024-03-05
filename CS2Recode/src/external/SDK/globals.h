#pragma once
#include <Windows.h>
#include <string>
#include <map>
#include <unordered_map>

namespace vars {
	bool espEnabled = true;
	float aimbotFov = 30.f;
	bool aimbotEnable = false;
	float espCol[] = {1,1,1,1};
	int aimKey = VK_LMENU;

	std::unordered_map<std::string, int> WeaponToHitBox = {
	{"awp",bones::spine},
	{"ak47",bones::head},
	{"aug",bones::head},
	{"m4a1",bones::head},
	{"m4a1_silencer",bones::head},
	{"sg556",bones::head},
	{"hkp2000",bones::head},
	{"p250",bones::head},
	{"glock",bones::head},
	{"deagle",bones::head},
	{"revolver",bones::head},
	{"elite",bones::head},
	{"ssg08",bones::spine},
	{"fiveseven",bones::head},
	{"tec9",bones::head},
	{"cz75a",bones::head}
	};
	

}