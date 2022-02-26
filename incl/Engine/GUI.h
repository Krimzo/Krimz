#pragma once

#include "ImGui/imgui.h"

#include "Engine/Engine.h"


// Panel sizes
inline const float sidePanelsWidth = 0.2f;
inline const float explorerHeight = 0.2f;

// Size buffers
inline kl::ivec2 winSize;
inline ImVec2 mainMenuSize;

// GUI functions
void MainMenu();
void Scene();
void Explorer();
void Properties();