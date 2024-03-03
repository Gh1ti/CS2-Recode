#include "textureloader.h"

#ifdef TEXTURE_H
bool TextureLoader::CreateTextureFromMemory(ID3D11Device* device, 
    const void* rawData, 
    size_t dataSize, 
    ID3D11ShaderResourceView** textureSRV) {
    D3DX11_IMAGE_LOAD_INFO params = {};

    ID3D11Resource* tempTexture = nullptr;
    HRESULT hr = D3DX11CreateTextureFromMemory(device, rawData, static_cast<UINT>(dataSize), &params, nullptr, &tempTexture, nullptr);
    if (FAILED(hr)) {
        return false;
    }

    hr = device->CreateShaderResourceView(tempTexture, nullptr, textureSRV);
    if (FAILED(hr)) {
        tempTexture->Release();
        return false;
    }
    tempTexture->Release();

    return true;
}
#endif
