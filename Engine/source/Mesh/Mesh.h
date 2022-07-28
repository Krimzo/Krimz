#pragma once

#include "KrimzLib.h"
#include "Named/Named.h"
#include "PxPhysicsAPI.h"

using namespace physx;


namespace Krimz
{
	class Mesh : public Named
	{
		kl::ref<kl::gpu> m_GPU;

	public:
		std::vector<kl::vertex> vertices;
		kl::dx::buffer buffer = nullptr;
		PxTriangleMesh* cooked = nullptr;

		Mesh(const std::string& name, kl::ref<kl::gpu> gpu, PxCooking* cooking, PxPhysics* physics, const std::vector<kl::vertex>& vertices);
		Mesh(const Krimz::Mesh&) = delete;
		void operator=(const Krimz::Mesh&) = delete;
		~Mesh();
	};
}
