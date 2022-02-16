#pragma once

#include "KrimzLib/KrimzLib.hpp"

#include "Engine/Struct.h"


// Window
static kl::window win;

// Buffers
static kl::pbuffer<kl::entity> entities;
static kl::pbuffer<kl::skybox> skyboxes;

// View
static kl::color background = kl::colors::gray;
static kl::camera camera;

// Lighting
static kl::ambient ambient;
static kl::direct sun;

// Engine skybox
static kl::skybox* skybox = nullptr;

// Highlight
static kl::color highlight = kl::colors::white;
static kl::entity* selected = nullptr;

// DirectX
static kl::gpu* gpu = nullptr;
static kl::raster* solid_ra = nullptr;
static kl::raster* wire_ra = nullptr;
static kl::shaders* default_sh = nullptr;
static kl::shaders* highlight_sh = nullptr;

// Window callbacks
void Start();
void Input();
void Update();
void End();
