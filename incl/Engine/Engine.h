#pragma once

#include "KrimzLib/KrimzLib.hpp"

#include "Engine/Struct.h"
#include "Engine/Rasters.h"
#include "Engine/DStates.h"
#include "Engine/Shaders.h"
#include "Engine/CBuffers.h"
#include "Engine/Callbacks.h"
#include "Engine/Picking.h"
#include "Engine/Outline.h"
#include "Engine/Gizmo.h"


// Window
inline kl::window win;

// Gpu
inline kl::gpu* gpu = nullptr;

// Entities
inline kl::pbuffer<kl::entity> entities;

// View
inline kl::color background = kl::colors::gray;
inline kl::camera camera;

// Lighting
inline kl::ambient ambient;
inline kl::direct sun;

// Engine skybox
inline kl::skybox* skybox = nullptr;
inline kl::pbuffer<kl::skybox> skyboxes;

// Time
inline kl::timer timer;
inline float deltaT = 0;
inline float elapsedT = 0;
