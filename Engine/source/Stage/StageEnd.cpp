#include "Stage/Stage.h"
#include "Render/Render.h"
#include "Types/Entity.h"
#include "Types/Skybox.h"
#include "Types/Texture.h"
#include "Types/Mesh.h"
#include "Scripting/Scripting.h"


void Krimz::Stage::End() {
	Krimz::gpu = nullptr;
}
