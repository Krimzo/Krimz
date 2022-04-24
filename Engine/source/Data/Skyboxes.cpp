#include "Data/Skyboxes.h"

#include "Render/Render.h"
#include "Logging/Logging.h"


void FixSkyboxName(String& name) {
	const String nameCopy = name;
	int counter = 0;
	while (Engine::find(Engine::skyboxes, name)) {
		name = nameCopy + "_" + std::to_string(++counter);
	}
}

// Skybox box vertices
static const std::vector<kl::vertex> boxVertices = {
	kl::vertex(kl::float3(1, -1,  1)), kl::vertex(kl::float3(1, -1, -1)), kl::vertex(kl::float3(1,  1, -1)),
	kl::vertex(kl::float3(1,  1, -1)), kl::vertex(kl::float3(1,  1,  1)), kl::vertex(kl::float3(1, -1,  1)),
	kl::vertex(kl::float3(-1, -1,  1)), kl::vertex(kl::float3(-1,  1,  1)), kl::vertex(kl::float3(-1,  1, -1)),
	kl::vertex(kl::float3(-1,  1, -1)), kl::vertex(kl::float3(-1, -1, -1)), kl::vertex(kl::float3(-1, -1,  1)),
	kl::vertex(kl::float3(-1,  1,  1)), kl::vertex(kl::float3(1,  1,  1)), kl::vertex(kl::float3(1,  1, -1)),
	kl::vertex(kl::float3(1,  1, -1)), kl::vertex(kl::float3(-1,  1, -1)), kl::vertex(kl::float3(-1,  1,  1)),
	kl::vertex(kl::float3(-1, -1,  1)), kl::vertex(kl::float3(-1, -1, -1)), kl::vertex(kl::float3(1, -1, -1)),
	kl::vertex(kl::float3(1, -1, -1)), kl::vertex(kl::float3(1, -1,  1)), kl::vertex(kl::float3(-1, -1,  1)),
	kl::vertex(kl::float3(-1, -1,  1)), kl::vertex(kl::float3(1, -1,  1)), kl::vertex(kl::float3(1,  1,  1)),
	kl::vertex(kl::float3(1,  1,  1)), kl::vertex(kl::float3(-1,  1,  1)), kl::vertex(kl::float3(-1, -1,  1)),
	kl::vertex(kl::float3(-1, -1, -1)), kl::vertex(kl::float3(-1,  1, -1)), kl::vertex(kl::float3(1,  1, -1)),
	kl::vertex(kl::float3(1,  1, -1)), kl::vertex(kl::float3(1, -1, -1)), kl::vertex(kl::float3(-1, -1, -1))
};

// Shaders
static const String shaders =
R"(
// Vertex shader
struct vOut {
    float4 world : SV_POSITION;
    float3 textur : TEX;
};

cbuffer SB_CB : register(b0) {
    matrix vp;
}

vOut vShader(float3 pos : POS_IN, float2 tex : TEX_IN, float3 norm : NORM_IN) {
    vOut data;
    data.world = mul(float4(pos, 0.0f), vp).xyww;
    data.textur = pos;
    return data;
}

// Pixel shader
SamplerState samp : register(s0);
TextureCube tex0 : register(t0);

float4 pShader(vOut data) : SV_TARGET {
    return tex0.Sample(samp, data.textur);
}
)";

// Constr/destr
Engine::Skybox::Skybox(const String& name, const kl::image& front, const kl::image& back, const kl::image& left, const kl::image& right, const kl::image& top, const kl::image& bottom) : EObject(name) {
	// Name fix
	FixSkyboxName(this->name);

	// Compiling skybox shaders
	sky_vtx = Engine::Render::gpu->newVertexShader(shaders);
	sky_pxl = Engine::Render::gpu->newPixelShader(shaders);
	vtx_cb = Engine::Render::gpu->newConstBuffer(sizeof(kl::mat4));

	// Generating the box mesh
	box_mes = Engine::Render::gpu->newVertBuffer(boxVertices);

	// Generating the box texture
	ID3D11Texture2D* boxTex = Engine::Render::gpu->newTexture(front, back, left, right, top, bottom);
	box_tex = Engine::Render::gpu->newShaderView(boxTex);
	Engine::Render::gpu->destroy(boxTex);
}
Engine::Skybox::Skybox(const String& name, const kl::image& fullbox) : EObject(name) {
	// Checking the aspect ratio
	if (fullbox.width() % 4 == 0 && fullbox.height() % 3 == 0) {
		// Getting the part size
		const int partWidth = fullbox.width() / 4;
		const int partHeight = fullbox.height() / 3;

		// Checking the part size
		if (partWidth == partHeight) {
			const kl::int2 partSize(partWidth, partHeight);
			const kl::image front = fullbox.rect(partSize * kl::int2(1, 1), partSize * kl::int2(2, 2));
			const kl::image back = fullbox.rect(partSize * kl::int2(3, 1), partSize * kl::int2(4, 2));
			const kl::image left = fullbox.rect(partSize * kl::int2(0, 1), partSize * kl::int2(1, 2));
			const kl::image right = fullbox.rect(partSize * kl::int2(2, 1), partSize * kl::int2(3, 2));
			const kl::image top = fullbox.rect(partSize * kl::int2(1, 0), partSize * kl::int2(2, 1));
			const kl::image bottom = fullbox.rect(partSize * kl::int2(1, 2), partSize * kl::int2(2, 3));
			this->Skybox::Skybox(name, front, back, left, right, top, bottom);
		}
		else {
			Engine::log("Texture file \"" + name + "\" has unspported ratio!");
			valid = false;
		}
	}
	else {
		Engine::log("Texture file \"" + name + "\" has unspported ratio!");
		valid = false;
	}
}
Engine::Skybox::Skybox(const Engine::Skybox& sb) : EObject(sb.name) {
	sky_vtx = sb.sky_vtx;
	sky_pxl = sb.sky_pxl;
	vtx_cb = sb.vtx_cb;
	box_mes = sb.box_mes;
	box_tex = sb.box_tex;
	valid = sb.valid;
	((Engine::Skybox*)&sb)->canDelete = false;
	canDelete = true;
}
Engine::Skybox::~Skybox() {
	if (valid && canDelete) {
		Engine::Render::gpu->destroy(sky_vtx);
		Engine::Render::gpu->destroy(sky_pxl);
		Engine::Render::gpu->destroy(vtx_cb);
		Engine::Render::gpu->destroy(box_mes);
		Engine::Render::gpu->destroy(box_tex);
	}
}

// Valid
bool Engine::Skybox::isValid() const {
	return valid;
}

// Render
void Engine::Skybox::render(const kl::mat4& vpMat) const {
	// Shader bind
	Engine::Render::gpu->bind(sky_pxl);
	Engine::Render::gpu->bind(sky_vtx);
	Engine::Render::gpu->bindVertCBuff(vtx_cb, 0);
	Engine::Render::gpu->setBuffData(vtx_cb, (void*)&vpMat);

	// Binding the texture
	Engine::Render::gpu->bindPixlTex(box_tex, 0);

	// Drawing the cubemap
	Engine::Render::gpu->draw(box_mes);
}

// Checks the buffer for the name
bool Engine::find(const std::list<Engine::Skybox>& skyboxes, const String& name) {
	for (auto& skybox : skyboxes) {
		if (skybox.name == name) {
			return true;
		}
	}
	return false;
}
