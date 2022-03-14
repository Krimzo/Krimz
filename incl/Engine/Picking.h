#pragma once

#include "KrimzLib/KrimzLib.hpp"


inline int heldIndex = -1;
inline int mouseIndex = -1;
inline int selectedInd = -1;
inline kl::entity* selected = nullptr;
inline ID3D11Texture2D* pickingTex = nullptr;
inline ID3D11Texture2D* pickingTexST = nullptr;
inline ID3D11RenderTargetView* pickingTargetV = nullptr;
