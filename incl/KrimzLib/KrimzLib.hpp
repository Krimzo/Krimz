#pragma once

// Including math
#include "KrimzLib/math/math.h"
#include "KrimzLib/math/int2.h"
#include "KrimzLib/math/float2.h"
#include "KrimzLib/math/float3.h"
#include "KrimzLib/math/float4.h"
#include "KrimzLib/math/mat3.h"
#include "KrimzLib/math/mat4.h"

// Including color
#include "KrimzLib/color/color.h"
#include "KrimzLib/color/image.h"

// Including utility
#include "KrimzLib/utility/time.h"
#include "KrimzLib/utility/convert.h"
#include "KrimzLib/utility/random.h"
#include "KrimzLib/utility/console.h"
#include "KrimzLib/utility/file.h"
#include "KrimzLib/utility/web.h"
#include "KrimzLib/utility/thread.h"

// Including memory
#include "KrimzLib/memory/pbuffer.h"

// Including the geometry
#include "KrimzLib/geometry/ray.h"
#include "KrimzLib/geometry/plane.h"
#include "KrimzLib/geometry/sphere.h"
#include "KrimzLib/geometry/vertex.h"
#include "KrimzLib/geometry/triangle.h"

// Including window
#include "KrimzLib/window/keys.h"
#include "KrimzLib/window/mouse.h"
#include "KrimzLib/window/window.h"

// Including dx
#include "KrimzLib/dx/gpu.h"

#ifdef KL_USING_IMGUI
// Including ImGui
#include "KrimzLib/igui/igui.h"
#endif

// Including the light
#include "KrimzLib/light/ambient.h"
#include "KrimzLib/light/direct.h"

// Including the entity
#include "KrimzLib/entity/entity.h"

// Including the renderer
#include "KrimzLib/renderer/camera.h"
#include "KrimzLib/renderer/skybox.h"
#include "KrimzLib/renderer/renderer.h"
