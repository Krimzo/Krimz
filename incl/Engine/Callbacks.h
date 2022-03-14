#pragma once

#include "KrimzLib/KrimzLib.hpp"


// Engine stages
void Start();
void Update();
void Resize(const kl::int2& siz);
void End();

// Update stages
void Shadows();
void Draw();
void Outline();
void Gizmo();
void GUI();
