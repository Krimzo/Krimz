#include "KrimzLib/dx/gpu.h"


ID3D11ShaderResourceView* kl::gpu::newShaderView(ID3D11Texture2D* tex) {
    // Creating the render target view
    ID3D11ShaderResourceView* shaderView = nullptr;
    device->CreateShaderResourceView(tex, nullptr, &shaderView);
    if (!shaderView) {
        std::cout << "DirectX: Could not create a backbuffer!";
        std::cin.get();
        exit(69);
    }

    // Saving child
    childs.push_back(shaderView);

    // Return
    return shaderView;
}

// Binds the texture
void kl::gpu::bindPixlView(ID3D11ShaderResourceView* buff, int slot) {
    devcon->PSSetShaderResources(slot, 1, &buff);
}
