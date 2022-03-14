#pragma once

#include "KrimzLib/KrimzLib.hpp"


inline const float gizmoScale = 0.25f;

enum GIZMO_TYPE {
	GIZMO_NONE = 0,
	GIZMO_SCALE,
	GIZMO_MOVE,
	GIZMO_ROTATE
};
inline int chosenGizmo = GIZMO_NONE;
inline ID3D11Buffer* gizmo_scale = nullptr;
inline ID3D11Buffer* gizmo_move = nullptr;
inline ID3D11Buffer* gizmo_rotate = nullptr;
inline kl::float4 gizmoColX = kl::color(205, 55, 75);
inline kl::float4 gizmoColY = kl::color(115, 175, 40);
inline kl::float4 gizmoColZ = kl::color(55, 120, 205);
