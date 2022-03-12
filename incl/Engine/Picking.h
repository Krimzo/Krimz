#pragma once

#include "KrimzLib/KrimzLib.hpp"


inline int selectedInd = -2;
inline kl::entity* selected = nullptr;
inline ID3D11Texture2D* pickingTex = nullptr;
inline ID3D11Texture2D* pickingTexST = nullptr;
inline ID3D11RenderTargetView* pickingTargetV = nullptr;
