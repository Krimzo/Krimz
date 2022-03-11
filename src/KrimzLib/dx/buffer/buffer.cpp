#include "KrimzLib/dx/gpu.h"


ID3D11Buffer* kl::gpu::newBuffer(D3D11_BUFFER_DESC* desc, D3D11_SUBRESOURCE_DATA* subData = nullptr) {
    // Buffer creation
    ID3D11Buffer* constBuff = nullptr;
    device->CreateBuffer(desc, subData, &constBuff);
    if (!constBuff) {
        std::cout << "DirectX: Could not create a constant buffer!";
        std::cin.get();
        exit(69);
    }

    // Saving child
    childs.push_back(constBuff);

    // Return
    return constBuff;
}
