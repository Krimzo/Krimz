#pragma once

#include "KrimzLib/KrimzLib.hpp"

#include "Engine/Struct.h"


// Window
inline kl::window win;

// Buffers
inline kl::pbuffer<kl::entity> entities;
inline kl::pbuffer<kl::skybox> skyboxes;

// View
inline kl::color background = kl::colors::gray;
inline kl::camera camera;

// Lighting
inline kl::ambient ambient;
inline kl::direct sun;

// Engine skybox
inline kl::skybox* skybox = nullptr;

// Highlight
inline kl::color highlight = kl::colors::white;
inline kl::entity* selected = nullptr;

// Time
inline kl::timer timer;
inline float deltaT = 0;
inline float elapsedT = 0;

// DirectX
inline kl::gpu* gpu = nullptr;
inline kl::raster* solid_ra = nullptr;
inline kl::raster* wire_ra = nullptr;
inline kl::shaders* editor_sh = nullptr;
inline kl::shaders* highlight_sh = nullptr;

// Frame stages
void Start();
void Input();
void Draw();
void Highlight();
void GUI();
void Update();
void End();
