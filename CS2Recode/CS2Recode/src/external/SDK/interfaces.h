#pragma once
#include "Memory/CMemory.h"
#include "CMatrix.h"
#include "Memory/Offsets.h"
namespace interfaces {
	CMemory mem = CMemory("cs2.exe");
	uintptr_t client = mem.GetModuleAddress("client.dll");
	CMatrix4x4 vm = mem.Read<CMatrix4x4>(client + offsets::dwViewMatrix);
	uintptr_t dwEntityList = mem.Read<uintptr_t>(client + offsets::dwEntityList);
}