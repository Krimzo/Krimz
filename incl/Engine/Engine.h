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
inline kl::color outline = kl::colors::orange;
inline kl::entity* selected = nullptr;

// Time
inline kl::timer timer;
inline float deltaT = 0;
inline float elapsedT = 0;

// DirectX
inline kl::gpu* gpu = nullptr;

// Rasterizers
inline kl::raster* solid_ra = nullptr;
inline kl::raster* wire_ra = nullptr;
inline kl::raster* shadow_ra = nullptr;

// Shaders
inline kl::shaders* editor_sh = nullptr;
inline kl::shaders* shadow_sh = nullptr;
inline kl::shaders* outline_sh = nullptr;
inline kl::shaders* gizmo_sh = nullptr;

// Gizmo meshes
enum GIZMO_TYPE {
	GIZMO_NONE = 0,
	GIZMO_SCALE,
	GIZMO_MOVE,
	GIZMO_ROTATE
};
inline int chosenGizmo = GIZMO_NONE;
inline kl::mesh* gizmo_scale = nullptr;
inline kl::mesh* gizmo_move = nullptr;
inline kl::mesh* gizmo_rotate = nullptr;

// Frame stages
void Start();
void Input();
void Shadows();
void Draw();
void Outline();
void Gizmo();
void GUI();
void Update();
void End();
