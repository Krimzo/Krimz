#pragma once

#include "Engine/Engine.h"


// Panel flags
inline const int panelWinFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse;

// Panel sizes
inline const float sidePanelsRatio = 0.18f;
inline const float explorerHeightRatio = 0.20f;

// Size buffers
inline kl::int2 winSize;
inline ImVec2 mainMenuSize;

// Scene
inline kl::int2 guiScenePos;
inline kl::int2 guiSceneSize;

// Explorer
inline kl::int2 guiExplorPos;
inline kl::int2 guiExplorSize;

// Properties
inline kl::int2 guiPropsPos;
inline kl::int2 guiPropsSize;

// Viewport
inline kl::int2 guiViewportPos;
inline kl::int2 guiViewportSize;

// GUI functions
void MainMenu();
void Scene();
void Explorer();
void Properties();
