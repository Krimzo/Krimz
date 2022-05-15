#include "Types/Skybox.h"
#include "Render/Render.h"
#include "Logging/Logging.h"
#include "Types/Mesh.h"


kl::shaders Engine::Skybox::shaders = {};

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

// Constr/destr
Engine::Skybox::Skybox(const String& name, const kl::image& front, const kl::image& back, const kl::image& left, const kl::image& right, const kl::image& top, const kl::image& bottom) : Named(Named::Type::Skybox, name) {
	ID3D11Texture2D* boxTex = Engine::Render::gpu->newTexture(front, back, left, right, top, bottom);
	shaderV = Engine::Render::gpu->newShaderView(boxTex);
	Engine::Render::gpu->destroy(boxTex);
}
Engine::Skybox::Skybox(const String& name, const kl::image& fullbox) : Named(Named::Type::Skybox) {
	if (fullbox.width() % 4 == 0 && fullbox.height() % 3 == 0) {
		const int partWidth = fullbox.width() / 4;
		const int partHeight = fullbox.height() / 3;
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
	}
	if (!shaderV) {
		Engine::log("Texture file \"" + name + "\" has unspported ratio!");
	}
}
Engine::Skybox::~Skybox() {
	if (Engine::Render::gpu && isValid()) {
		Engine::Render::gpu->destroy(shaderV);
	}
}

bool Engine::Skybox::isValid() const {
	return bool(shaderV);
}

void Engine::Skybox::render(const kl::mat4& vpMat) const {
	if (shaderV) {
		// Shaders
		Engine::Render::gpu->bind(shaders);

		// CBUffer
		Engine::Render::gpu->autoVertexCBuffer(vpMat);

		// Binding the texture
		Engine::Render::gpu->bindPixelShaderView(shaderV, 0);

		// Drawing the cubemap
		Engine::Render::gpu->draw(Engine::Default::cube->buffer);
	}
}
