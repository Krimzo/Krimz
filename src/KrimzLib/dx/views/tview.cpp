#include "KrimzLib/dx/gpu.h"


ID3D11RenderTargetView* kl::gpu::newTargetView(ID3D11Texture2D* tex) {
    // Creating the render target view
    ID3D11RenderTargetView* targetView = nullptr;
    device->CreateRenderTargetView(tex, nullptr, &targetView);
    if (!targetView) {
        std::cout << "DirectX: Could not create a backbuffer!";
        std::cin.get();
        exit(69);
    }

    // Saving child
    childs.push_back(targetView);

    // Return
    return targetView;
}

void kl::gpu::clear(ID3D11RenderTargetView* view, const kl::float4& color) {
    devcon->ClearRenderTargetView(view, (float*)&color);
}
