#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#pragma comment(lib, "d3dx11.lib")

#ifndef TEXTURE_H
#define TEXTURE_H

namespace TextureLoader {
	bool CreateTextureFromMemory(ID3D11Device* device, 
		const void* rawData, 
		size_t dataSize, 
		ID3D11ShaderResourceView** textureSRV);
}


#endif
