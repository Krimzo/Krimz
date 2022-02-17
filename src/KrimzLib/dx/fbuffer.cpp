#include "KrimzLib/dx/fbuffer.h"


// Constructor
kl::fbuffer::fbuffer(IDXGISwapChain* chain, ID3D11Device* dev, ID3D11DeviceContext* devcon, int width, int height) {
    // Saving devcon
    this->devcon = devcon;

    // Saving the size
    this->width = width;
    this->height = height;

    // Getting the back buffer address
    ID3D11Texture2D* buffAddrs = nullptr;
    chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffAddrs);
    if (!buffAddrs) {
        std::cout << "DirectX: Could not get back buffer address!";
        std::cin.get();
        exit(69);
    }

    // Creating the backbuffer
    dev->CreateRenderTargetView(buffAddrs, nullptr, &renderTargets[0]);
    if (!renderTargets[0]) {
        std::cout << "DirectX: Could not create a backbuffer!";
        std::cin.get();
        exit(69);
    }

    // Creating the picking texture
    D3D11_TEXTURE2D_DESC pickTexDes = {};
    pickTexDes.Width = width;
    pickTexDes.Height = height;
    pickTexDes.MipLevels = 1;
    pickTexDes.ArraySize = 1;
    pickTexDes.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    pickTexDes.SampleDesc.Count = 1;
    pickTexDes.Usage = D3D11_USAGE_DEFAULT;
    pickTexDes.BindFlags = D3D11_BIND_RENDER_TARGET;
    dev->CreateTexture2D(&pickTexDes, nullptr, &pickTex);
    if (!pickTex) {
        std::cout << "DirectX: Could not create a picking texture!";
        std::cin.get();
        exit(69);
    }

    // Creating the picking render target
    dev->CreateRenderTargetView(pickTex, nullptr, &renderTargets[1]);
    if (!renderTargets[1]) {
        std::cout << "DirectX: Could not create a picking render target!";
        std::cin.get();
        exit(69);
    }

    // Creating the staging texture
    D3D11_TEXTURE2D_DESC stagTexDes = {};
    stagTexDes.Width = 1;
    stagTexDes.Height = 1;
    stagTexDes.MipLevels = 1;
    stagTexDes.ArraySize = 1;
    stagTexDes.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    stagTexDes.SampleDesc.Count = 1;
    stagTexDes.Usage = D3D11_USAGE_STAGING;
    stagTexDes.BindFlags = NULL;
    stagTexDes.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    dev->CreateTexture2D(&stagTexDes, nullptr, &stagingTex);
    if (!stagingTex) {
        std::cout << "DirectX: Could not create a staging texture!";
        std::cin.get();
        exit(69);
    }

    // Creating depth/stencil states
    D3D11_DEPTH_STENCIL_DESC depthDesc_e = {};
    depthDesc_e.DepthEnable = true;
    depthDesc_e.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthDesc_e.DepthFunc = D3D11_COMPARISON_LESS;
    depthDesc_e.StencilEnable = false;
    depthDesc_e.StencilEnable = true;
    depthDesc_e.StencilReadMask = 0xFF;
    depthDesc_e.StencilWriteMask = 0xFF;
    depthDesc_e.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthDesc_e.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthDesc_e.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthDesc_e.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    depthDesc_e.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthDesc_e.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthDesc_e.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthDesc_e.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    dev->CreateDepthStencilState(&depthDesc_e, &dsState_enabled);
    if (!dsState_enabled) {
        std::cout << "DirectX: Could not create a depth/stencil state!";
        std::cin.get();
        exit(69);
    }
    D3D11_DEPTH_STENCIL_DESC depthDesc_d = {};
    depthDesc_d.DepthEnable = false;
    depthDesc_d.StencilEnable = false;
    dev->CreateDepthStencilState(&depthDesc_d, &dsState_disabled);
    if (!dsState_disabled) {
        std::cout << "DirectX: Could not create a depth/stencil state!";
        std::cin.get();
        exit(69);
    }

    // Creating depth/stencil textures
    D3D11_TEXTURE2D_DESC depthTexDesc = {};
    depthTexDesc.Width = width;
    depthTexDesc.Height = height;
    depthTexDesc.MipLevels = 1;
    depthTexDesc.ArraySize = 1;
    depthTexDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthTexDesc.SampleDesc.Count = 1;
    depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
    depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    ID3D11Texture2D* depthTex = nullptr;
    dev->CreateTexture2D(&depthTexDesc, NULL, &depthTex);
    if (!depthTex) {
        std::cout << "DirectX: Could not create a depth/stencil buffer texture!";
        std::cin.get();
        exit(69);
    }

    // Creating depth/stencil buffers
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Format = depthTexDesc.Format;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
    dev->CreateDepthStencilView(depthTex, &dsvDesc, &dsBuff);
    if (!dsBuff) {
        std::cout << "DirectX: Could not create a depth/stencil buffer view!";
        std::cin.get();
        exit(69);
    }

    // Cleanup
    depthTex->Release();
    buffAddrs->Release();
}

// Destructor
kl::fbuffer::~fbuffer() {
    dsState_enabled->Release();
    dsState_disabled->Release();
    dsBuff->Release();
    pickTex->Release();
    stagingTex->Release();
    renderTargets[0]->Release();
    renderTargets[1]->Release();
}

// Binds the buffer
void kl::fbuffer::bind(bool depthTest) {
    devcon->OMSetDepthStencilState(depthTest ? dsState_enabled : dsState_disabled, 1);
    devcon->OMSetRenderTargets(2, renderTargets, dsBuff);
}

// Clears the buffer
void kl::fbuffer::clearColor(const kl::vec4& color) {
    // Picking target reset value
    static const int pickResetValue = -1;
    static const kl::vec4 pickCol(*(float*)&pickResetValue, 0, 0, 0);

    // Clearing the render targets
    devcon->ClearRenderTargetView(renderTargets[0], (float*)&color);
    devcon->ClearRenderTargetView(renderTargets[1], (float*)&pickCol);
}
void kl::fbuffer::clearDepth() {
    devcon->ClearDepthStencilView(dsBuff, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}

// Returns the picking index
int kl::fbuffer::getPickingIndex(const kl::ivec2& pos) {
    if (pos.x >= 0 && pos.x < this->width && pos.y >= 0 && pos.y < this->height) {
        // Copying the picking texture
        D3D11_BOX srcBox = {};
        srcBox.left = pos.x;
        srcBox.right = srcBox.left + 1;
        srcBox.top = pos.y;
        srcBox.bottom = srcBox.top + 1;
        srcBox.front = 0;
        srcBox.back = 1;
        devcon->CopySubresourceRegion(stagingTex, 0, 0, 0, 0, pickTex, 0, &srcBox);

        // Mapping the staging texture
        float index = 0;
        D3D11_MAPPED_SUBRESOURCE msr = {};
        devcon->Map(stagingTex, 0, D3D11_MAP_READ, NULL, &msr);
        memcpy(&index, msr.pData, sizeof(float));
        devcon->Unmap(stagingTex, NULL);

        // Getting the index
        return *(int*)&index;
    }
    return -2;
}
